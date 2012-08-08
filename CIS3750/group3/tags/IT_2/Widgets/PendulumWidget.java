/*
 * Pendulum Widget
 *
 * Written by Filip Anguelov
 * ID: 0223807
 * E-mail: fanguelo@uoguelph.ca
 * 
 * This widget simulates a pendulum
 * 
 * A ball attached to a stationary box that swings on a joint
 * 
 */
package Widgets;

import java.awt.Graphics2D;

import javax.swing.ImageIcon;

import net.phys2d.math.Vector2f;
import net.phys2d.raw.BasicJoint;
import net.phys2d.raw.Body;
import net.phys2d.raw.CollisionEvent;
import net.phys2d.raw.Joint;


/**
 * The Class PendulumWidget.
 * 
 * @author Filip Anguelov
 */
public class PendulumWidget implements Widget
{
	
	private BodyBuilder buildBody = null; 
	private ObjectDrawer drawBody = null;
	
	private float swingForce = 25.0f;
	private float ballSize = 5.0f;
	private float boxSize = 5.0f;
	
	private Vector2f bobPosition = new Vector2f( 5f, 5f );
	private Vector2f anchorPosition = new Vector2f( 50, 5 );
	
	private Vector2f widgetPosition = new Vector2f( 0, 0 );
	
	private boolean isWidgetActive = false;
	private boolean isWidgetLocked = false;
	
	private Body[] bodyList = null;
	private BasicJoint[] jointList = null;
	
	/* the box surroundting the widget */
	private Vector2f[] boundingBox = null;

	private String widgetDescription = "This is a pendulum widget, simply a ball on a rope that hits things.";
	private String widgetName = "Pendulum";
	
	private Direction currentDirection = Direction.EAST;
	
	/**
	 * Instantiates a new pendulum widget.
	 */
	public PendulumWidget()
	{
		/* init the objects */
		this.buildBody = new BodyBuilder();
		this.drawBody = new ObjectDrawer();
		
		
		/* set up the temp body array */
		Body[] temp = new Body[2];
		
		/* creates the swinging ball */
		Body bob = this.buildBody.makeCircle( "Bob", this.ballSize, this.bobPosition.x, this.bobPosition.y, 140, false );
		
		bob.setGravityEffected(false);
		
		/*
		 * Setting restitution adjusts the bounciness of the object
		 * 
		 * Referenced from:
		 * 
		 * Author: kevglass and Jon
		 * 
		 * URL: http://slick.javaunlimited.net/viewtopic.php?p=1914&sid=e9cf30d10431a034c16609556d227c48 
		 */
		bob.setRestitution(-0.5f);
		bob.setDamping(-0.5f);
		bob.canRest();
		
		/* create the box that we will attach the ball to */
		Body anchor = this.buildBody.makeBox( "anchor", this.boxSize, this.boxSize, this.anchorPosition.x, this.anchorPosition.y, 20, true );
		
		/* add the bodies to the array */
		temp[0] = anchor;
		temp[1] = bob;
		
		/* assign temp to the global */
		this.bodyList = temp;
		
		/* create a joint linking the anchor and the hitter */
		BasicJoint joint = this.buildBody.connectBody(this.bodyList[1], this.bodyList[0]);
		
		/* create joint list and add*/
		this.jointList = new BasicJoint[1];
		this.jointList[0] = joint;
		
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#activateWidget()
	 */
	public void activateWidget( )
	{
		/* IF the widget is active then dont activate */
		if( this.isActive() == false )
		{
			if( this.currentDirection == Direction.EAST)
			{

				this.bodyList[1].setPosition( this.widgetPosition.x - 15, this.widgetPosition.y + 25 );
			}
			else
			{
				
				this.bodyList[1].setPosition( this.widgetPosition.x + 65, this.widgetPosition.y + 25 );
			}
			
			this.isWidgetActive = true;
			this.bodyList[1].setGravityEffected( true );
			this.bodyList[1].setDamping( 0f );
			this.bodyList[1].adjustVelocity( new Vector2f(  0, this.swingForce ) );
		}

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#attachJoint(net.phys2d.math.Vector2f)
	 */
	public Vector2f attachJoint( Vector2f point )
	{
		// TODO Auto-generated method stub
		
		/* this widget does not have joints that attach to it */
		return null;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#draw(java.awt.Graphics2D)
	 */
	public void draw( Graphics2D g )
	{
		/* here we call the drawer class to paint the object to screen*/
		
		this.drawBody.setGraphics( g );
		
		this.drawBody.drawBox( this.bodyList[0] );
		this.drawBody.drawCircle( this.bodyList[1] );

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getBodiesForSimulation()
	 */
	public Body[] getBodiesForSimulation( )
	{
		/* return the create objects */
		Body[] temp = this.bodyList;
		return temp;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getBoundary()
	 */
	public Vector2f[] getBoundary( )
	{
		/* here we create the vector2f array that defines the boundry of the widget */
		Vector2f boundary [] = 
		{ 
				new Vector2f ( this.widgetPosition.x, this.widgetPosition.y ), 						/* top-left corner 		*/
				new Vector2f ( this.widgetPosition.x + 50.0f, this.widgetPosition.y  ), 			/* top-right corner 	*/
				new Vector2f ( this.widgetPosition.x + 50.0f, this.widgetPosition.y + 50.0f ),		/* bottom-right corner 	*/
				new Vector2f ( this.widgetPosition.x, this.widgetPosition.y + 50.0f ) 				/* bottom-left corner 	*/
		};
		
		this.boundingBox = boundary;
		
		return boundary;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getDescription()
	 */
	public String getDescription( )
	{
		/* returns the description of the widget */
		return new String( this.widgetDescription );	
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getDirection()
	 */
	public Direction getDirection( )
	{
		/* return the direction */
		return this.currentDirection;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getIcon()
	 */
	public ImageIcon getIcon( )
	{
		/* create a new icon with the image of the widget */
		//return new ImageIcon( "pendulum_thumb.png" );
		return new ImageIcon("resources/icons/pendulum_thumb.png");
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getJointsForSimulation()
	 */
	public Joint[] getJointsForSimulation( )
	{
		/* rturn the joints for the sim */
		Joint temp[] = this.jointList;
		return temp;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getName()
	 */
	public String getName( )
	{
		/* return the widget name */
		return new String( this.widgetName );
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getPosition()
	 */
	public Vector2f getPosition( )
	{
		/* return the vector of the position */
		return this.widgetPosition;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getPositionX()
	 */
	public float getPositionX( )
	{
		/* return the x coordinate position of the widget */
		return this.widgetPosition.getX();
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getPositionY()
	 */
	public float getPositionY( )
	{
		/* return the y coordinate position of the widget */
		return this.widgetPosition.getY();
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#getType()
	 */
	public ActionType getType( )
	{
		/* this widget is an impact type widget */
		return ActionType.IMPACT;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#isActive()
	 */
	public boolean isActive( )
	{
		return new Boolean( this.isWidgetActive );
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#isConnectable()
	 */
	public boolean isConnectable( )
	{
		/* this widget is not connectable */
		return false;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#isLocked()
	 */
	public boolean isLocked( )
	{
		return new Boolean( this.isWidgetLocked );
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#reactToTouchingBody(net.phys2d.raw.CollisionEvent)
	 */
	public void reactToTouchingBody( CollisionEvent e )
	{
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#receiveImpulse(net.phys2d.math.Vector2f)
	 */
	public void receiveImpulse( Vector2f anchor_point )
	{
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#resetWidget()
	 */
	public void resetWidget( )
	{
		/* resets the widget to the starteing position */
		this.isWidgetActive = false;

		this.bodyList[1].setGravityEffected( false );
		this.bodyList[1].setPosition( this.widgetPosition.x - 15, this.widgetPosition.y + 25 );
		this.jointList[0].set( this.bodyList[1], this.bodyList[0], new Vector2f(this.bodyList[0].getPosition()) );
		this.bodyList[1].setDamping( 199f );
		this.bodyList[1].setIsResting(true);
			
		this.currentDirection = Direction.EAST;
		
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#rotateClockwise()
	 */
	public void rotateClockwise( )
	{
		/* both CCW and CW rotations are the same */
		this.rotateCounterClockwise();

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#rotateCounterClockwise()
	 */
	public void rotateCounterClockwise( )
	{
		/* if facing east face west and opposite */
		if( this.currentDirection == Direction.WEST )
		{
			this.setDirection( Direction.EAST );
			this.setPosition( new Vector2f( this.getPosition() ) );
		}
		else
		{
			this.setDirection( Direction.WEST );
			this.setPosition( new Vector2f( this.getPosition() ) );
	
		}
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setConnectionPoints(net.phys2d.math.Vector2f[])
	 */
	public void setConnectionPoints( Vector2f[] points )
	{
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setDirection(Widgets.Widget.Direction)
	 */
	public void setDirection( Direction d )
	{
			this.currentDirection = d;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setLock(boolean)
	 */
	public void setLock( boolean locked )
	{
		this.isWidgetLocked = locked;
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setPosition(net.phys2d.math.Vector2f)
	 */
	public void setPosition( Vector2f f )
	{
		/* set the position of the widget given a vector coordinate */
		/* must check if the widget is not locked and not active */
		if( this.isWidgetLocked == false && this.isWidgetActive != true )
		{
			/*update the vector */
			this.widgetPosition = f;
			
			/* update the bodies within */
			this.anchorPosition.y = f.y + 25;
			this.anchorPosition.x = f.x + 25;
			this.bodyList[0].setPosition( this.anchorPosition.x, this.anchorPosition.y );
			
			if( this.currentDirection == Direction.EAST)
			{
				/* move the ball according to EAST WEST direction */
				this.bobPosition.y = f.y + 25;
				this.bobPosition.x = f.x - 15;
				this.bodyList[1].setPosition( this.bobPosition.x, this.bobPosition.y );
				
			}else{
				
				this.bobPosition.y = f.y + 25;
				this.bobPosition.x = f.x + 65;
				this.bodyList[1].setPosition( this.bobPosition.x, this.bobPosition.y );
				
			}
			
			/* have to update the anchor because otherwise it causes errors in simulation */
			this.jointList[0].set( this.bodyList[1], this.bodyList[0], new Vector2f(this.bodyList[0].getPosition()) );
		}
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setPositionX(float)
	 */
	public void setPositionX( float x )
	{
		if( this.isWidgetLocked == false && this.isWidgetActive != true )
		{

			this.widgetPosition.x = x;
			
			/* the position of hte anchor is based on the size of the bounding box, which is 50.
			 * The box is palced in the middle
			 */
			this.anchorPosition.x = x + 25;
			this.bodyList[0].setPosition( this.anchorPosition.x, this.anchorPosition.y );
			
			if( this.currentDirection == Direction.EAST)
			{
				/* the position of the ball is based on the bounding box position and the upper left position */
				this.bobPosition.x = x - 15;
				this.bodyList[1].setPosition( this.bobPosition.x, this.bobPosition.y );
				
				
			}else{
				
				/* this sets the x posotion of the ball on the right side */
				this.bobPosition.x = x + 65;
				this.bodyList[1].setPosition( this.bobPosition.x, this.bobPosition.y );
				
			}
			
			this.jointList[0].set( this.bodyList[1], this.bodyList[0], new Vector2f(this.bodyList[0].getPosition()) );
			
		}
	}

	/* (non-Javadoc)
	 * @see Widgets.Widget#setPositionY(float)
	 */
	public void setPositionY( float y )
	{
		if( this.isWidgetLocked == false && this.isWidgetActive != true )
		{
			this.widgetPosition.y = y;
		
			/* placing the anchor so its vertically centered */
			this.anchorPosition.y = y + 25;
			this.bodyList[0].setPosition( this.anchorPosition.x, this.anchorPosition.y );
			
			/* same with the ball */
			this.bobPosition.y = y + 25;
			this.bodyList[1].setPosition( this.bobPosition.x, this.bobPosition.y );
			
			this.jointList[0].set( this.bodyList[1], this.bodyList[0], new Vector2f(this.bodyList[0].getPosition()) );
		}
	}

}
