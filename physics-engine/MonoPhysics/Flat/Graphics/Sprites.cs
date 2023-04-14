using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Flat.Graphics
{
    public sealed class Sprites
    {
        private bool isDisposed;
        private Game game;
        private SpriteBatch sprites;
        private BasicEffect effect;

        public Sprites(Game game)
        {
            if (game is null)
            {
                throw new ArgumentNullException("game");
            }

            this.game = game;
            this.isDisposed = false;
            this.sprites = new SpriteBatch(this.game.GraphicsDevice);

            this.effect = new BasicEffect(this.game.GraphicsDevice);
            this.effect.FogEnabled = false;
            this.effect.TextureEnabled = true;
            this.effect.LightingEnabled = false;
            this.effect.VertexColorEnabled = true;
            this.effect.World = Matrix.Identity;
            this.effect.Projection = Matrix.Identity;
            this.effect.View = Matrix.Identity;
        }

        public void Dispose()
        {
            if (this.isDisposed)
            {
                return;
            }

            this.sprites?.Dispose();
            this.isDisposed = true;
        }

        public void Begin(bool textureFilteringEnabled)
        {
            SamplerState sampler = textureFilteringEnabled ? SamplerState.LinearClamp : SamplerState.PointClamp;

            Viewport vp = this.game.GraphicsDevice.Viewport;
            this.effect.Projection = Matrix.CreateOrthographicOffCenter(0, vp.Width, 0, vp.Height, 0f, 1f);

            this.sprites.Begin(blendState: BlendState.AlphaBlend, samplerState: sampler, rasterizerState: RasterizerState.CullNone, effect: this.effect);
        }

        public void End()
        {
            this.sprites.End();
        }

        public void Draw(Texture2D texture, Vector2 origin, Vector2 position, Color color)
        {
            this.sprites.Draw(texture, position, null, color, 0f, origin, 1f, SpriteEffects.None, 0f);
        }
    }
}
