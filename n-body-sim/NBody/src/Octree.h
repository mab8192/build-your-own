#pragma once

#include <vector>
#include <array>
#include "raymath.h"
#include "Body.h"

class Octree {
public:
    // Constants
    static constexpr int MAX_BODIES = 8;        // Maximum bodies per leaf node
    static constexpr int MAX_DEPTH = 8;         // Maximum depth of the octree
    static constexpr float MIN_SIZE = 10.0f;    // Minimum size of an octree node

    // Structure to define a bounding box
    struct BoundingBox {
        Vector3 center;
        float halfSize;

        BoundingBox(Vector3 center, float halfSize)
            : center(center), halfSize(halfSize) {
        }

        bool contains(const Vector3& pos) const {
            return (pos.x >= center.x - halfSize && pos.x <= center.x + halfSize &&
                pos.y >= center.y - halfSize && pos.y <= center.y + halfSize &&
                pos.z >= center.z - halfSize && pos.z <= center.z + halfSize);
        }

        bool intersects(const BoundingBox& other) const {
            return (std::abs(center.x - other.center.x) <= halfSize + other.halfSize &&
                std::abs(center.y - other.center.y) <= halfSize + other.halfSize &&
                std::abs(center.z - other.center.z) <= halfSize + other.halfSize);
        }
    };

    // Constructor
    Octree(const BoundingBox& boundary, int depth = 0)
        : boundary(boundary), depth(depth) {
    }

    // Destructor
    ~Octree() {
        for (auto* child : children) {
            delete child;
        }
    }

    // Insert a body into the octree
    bool insert(Body* body) {
        // Check if this body is within this node's boundaries
        if (!boundary.contains(body->pos)) {
            return false;
        }

        // If we haven't reached capacity and we're a leaf, just add the body
        if (!divided && bodies.size() < MAX_BODIES) {
            bodies.push_back(body);
            return true;
        }

        // If we're at max depth, don't subdivide further
        if (!divided && depth < MAX_DEPTH && boundary.halfSize > MIN_SIZE) {
            subdivide();
        }

        // If we've subdivided, try to insert into children
        if (divided) {
            for (auto* child : children) {
                if (child->insert(body)) {
                    return true;
                }
            }
        }
        else {
            // We're at max depth or min size, so just add to this node
            bodies.push_back(body);
            return true;
        }

        return false;
    }

    // Find all bodies potentially colliding with the given body
    void queryPotentialCollisions(Body* body, float radius, std::vector<Body*>& result) {
        // Create a bounding box around the body with its radius
        BoundingBox queryBox({ body->pos.x, body->pos.y, body->pos.z }, radius);

        // Check if the query box intersects this node
        if (!boundary.intersects(queryBox)) {
            return;
        }

        // Check bodies in this node
        for (Body* other : bodies) {
            if (body != other && !other->deleted) {
                float distance = Vector3Distance(body->pos, other->pos);
                if (distance <= radius + other->radius) {
                    result.push_back(other);
                }
            }
        }

        // Check children if we've subdivided
        if (divided) {
            for (auto* child : children) {
                child->queryPotentialCollisions(body, radius, result);
            }
        }
    }

    // Find all bodies within a certain radius
    void queryRange(Vector3& position, float radius, std::vector<Body*>& result) {
        // Create a bounding box around the position
        BoundingBox queryBox({ position.x, position.y, position.z }, radius);

        // Check if the query box intersects this node
        if (!boundary.intersects(queryBox)) {
            return;
        }

        // Check bodies in this node
        for (Body* body : bodies) {
            if (!body->deleted) {
                float distance = Vector3Distance(position, body->pos);
                if (distance <= radius) {
                    result.push_back(body);
                }
            }
        }

        // Check children if we've subdivided
        if (divided) {
            for (auto* child : children) {
                child->queryRange(position, radius, result);
            }
        }
    }

    // Clear the tree and prepare for rebuilding
    void clear() {
        bodies.clear();
        if (divided) {
            for (auto* child : children) {
                delete child;
                child = nullptr;
            }
            divided = false;
        }
    }

    // Update the octree for a new frame (rebuild from scratch)
    static Octree* buildOctree(std::vector<Body>& bodies, float worldSize) {
        // Create a new root node
        Vector3 center = { 0, 0, 0 };
        Octree* root = new Octree(BoundingBox(center, worldSize));

        // Insert all bodies
        for (auto& body : bodies) {
            if (!body.deleted) {
                root->insert(&body);
            }
        }

        return root;
    }

    private:
        BoundingBox boundary;
        std::vector<Body*> bodies;
        std::array<Octree*, 8> children{ nullptr };
        bool divided = false;
        int depth;

        // Subdivide this octree into 8 children
        void subdivide() {
            float childSize = boundary.halfSize / 2;
            float offset = childSize;

            // Create 8 children in a specific order (bottom to top, left to right, back to front)
            for (int i = 0; i < 8; i++) {
                float x = boundary.center.x + (i & 1 ? offset : -offset);
                float y = boundary.center.y + (i & 2 ? offset : -offset);
                float z = boundary.center.z + (i & 4 ? offset : -offset);

                Vector3 childCenter = { x, y, z };
                children[i] = new Octree(BoundingBox(childCenter, childSize), depth + 1);
            }

            divided = true;

            // Redistribute bodies to children
            for (Body* body : bodies) {
                for (auto* child : children) {
                    if (child->boundary.contains(body->pos)) {
                        child->insert(body);
                        break;
                    }
                }
            }

            bodies.clear();
        }
};
