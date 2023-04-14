using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Runtime.CompilerServices;
using Flat.Graphics;

namespace Sandbox
{
    public class Game1 : Game
    {
        private GraphicsDeviceManager graphics;
        private Sprites sprites;
        private Texture2D texture;
        private Vector2 origin = Vector2.Zero;

        public Game1()
        {
            this.graphics = new GraphicsDeviceManager(this);
            this.graphics.SynchronizeWithVerticalRetrace = true;

            this.Content.RootDirectory = "Content";
            this.IsMouseVisible = true;
            this.IsFixedTimeStep = true;
        }

        protected override void Initialize()
        {
            this.sprites = new Sprites(this);

            base.Initialize();
        }

        protected override void LoadContent()
        {
            this.texture = this.Content.Load<Texture2D>("box");
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                Exit();
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            this.sprites.Begin(false);
            this.sprites.Draw(this.texture, this.origin, new Vector2(60, 60), Color.White);
            this.sprites.End();

            base.Draw(gameTime);
        }
    }
}
