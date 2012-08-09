import net.phys2d.math.*;
import net.phys2d.raw.*;
import net.phys2d.raw.shapes.*;


class ExplodingSprite extends Sprite
{
	public ExplodingSprite(String filename, double x, double y, double width, double height, double mass)
	{
		super(filename, x, y, width, height, mass);
	}
	
	public void explode()
	{
		double width = getHitBox().getSize().getX();
		double height = getHitBox().getSize().getY();
		if(width > 4 || height > 4)
		{
			int particles = (int) (Math.random() * width * 0.01 + 2); // (int) Math.max(Math.random() * width * 0.02 + width * 0.01, 2.0);
			for(int j = 0; j < particles; j++)
			{
				double w = (width * 0.45 * Math.random()) + width * 0.25;
				double h = (height * 0.45 * Math.random()) + height * 0.25;
				if(w < 3 || h < 3)
				{
					continue;
				}
				Sprite spr = new ExplodingSprite("icecube.png",
					getX() + (width * 0.25 * Math.random()) - (width * 0.25 * Math.random()),
					getY() - (height * 0.25 * Math.random()), w, h, 20.0f);

				spr.adjustAngularVelocity((float) (1 + Math.random() * 5));
				//spr.adjustAngularVelocity((float) (1 + Math.random() - Math.random()));
				spr.adjustVelocity(new Vector2f((float) (Math.random() * 5 - Math.random() * 5),
							(float) (Math.random() * 5 - Math.random() * 5)));
				spr.setRotation((float) (360 * Math.random()));
				spr.setWorld(getWorld());
				getWorld().add(spr);
			}
		}
		getWorld().remove(this);
	}

	public void update()
	{
		BodyList bodies = getTouching();
		double width = getHitBox().getSize().getX();
		double height = getHitBox().getSize().getY();

		if(getY() + height < -100)
		{
			getWorld().remove(this);
		}
		for(int i = 0; i < bodies.size(); i++)
		{
			Body body = bodies.get(i);
			if(body.getVelocity().lengthSquared() < 10 && getVelocity().lengthSquared() > 100)
			{
				explode();
				if(body instanceof ExplodingSprite)
				{
					((ExplodingSprite) body).explode();
				}
			}
		}
	}
}
