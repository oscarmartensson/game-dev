using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;      // Allows us to use SceneManager
using System.Collections.Generic;       // Needed for list

// --------------------------------
// Player inherits from MovingObject, our base class for objects that can move.
public class Player : MovingObject
{
    public GameObject gameManager;

    private Animator animator;                  // Used to store a reference to the Player's animator component.
    private AudioSource footSteps;
    private bool firing;

    // --------------------------------
    // Start overrides the Start function of MovingObject
    protected override void Start()
    {
        // Get a component reference to the Player's animator component
        animator = GetComponent<Animator>();
        footSteps = GetComponent<AudioSource>();

        // Call the Start function of the MovingObject base class.
        base.Start();
    }



    // --------------------------------
    // Update called each frame
    private void Update()
    {
        int horizontal = 0;     //Used to store the horizontal move direction.
        int vertical = 0;       //Used to store the vertical move direction.


        //Get input from the input manager, round it to an integer and store in horizontal to set x axis move direction
        horizontal = (int)(Input.GetAxisRaw("Horizontal"));
        Mathf.FloorToInt(horizontal);

        //Get input from the input manager, round it to an integer and store in vertical to set y axis move direction
        vertical = (int)(Input.GetAxisRaw("Vertical"));
        Mathf.FloorToInt(vertical);

        if (horizontal > 0 && !this.animator.IsInTransition(0) && !this.animator.GetCurrentAnimatorStateInfo(0).IsName("Player.jumpRight"))
        {
            Debug.Log("Animation called");
            animator.SetTrigger("rightTrigger"); 
            animator.SetBool("animPlaying", true);
            animator.SetBool("facingLeft", false);
        }
        else if (horizontal < 0 && !this.animator.IsInTransition(0) && !this.animator.GetCurrentAnimatorStateInfo(0).IsName("Player.jumpLeft"))
        {
            animator.SetTrigger("leftTrigger");
            animator.SetBool("animPlaying", true);
            animator.SetBool("facingLeft", true);
        }
        else
        {
            animator.SetBool("animPlaying", false);
        }

        //Check if moving horizontally, if so set vertical to zero.
        if (horizontal != 0)
        {
            vertical = 0;
        }

        //Check if we have a non-zero value for horizontal or vertical, and that we're inside the grid
        if (((horizontal > 0 && this.transform.position.x >= 0 && this.transform.position.x <= 8) ||
            (horizontal < 0 && this.transform.position.x <= 9 && this.transform.position.x > 0)  ||
            (vertical > 0 && this.transform.position.y >= 0 && this.transform.position.y < 5)    ||
            (vertical < 0 && this.transform.position.y <= 5 && this.transform.position.y > 0) )  &&
            (moveInProgress == false))
        {
            footSteps.Play();
            AttemptMove(horizontal, vertical);
        }

        if ((Input.GetButton("Fire1") || Input.GetKeyDown("space")) && !firing)
        {
            firing = true;
            animator.SetTrigger("attackTrigger");
            // Send a message to destroy nearby trees (if they exist)
            gameManager.GetComponent<BoardManager>().SendMessage("DestroyTrees", this.transform.position);
        }
        else
        {
            firing = false;
        }
    }

    // --------------------------------
    // AttemptMove overrides the AttemptMove function in the base class MovingObject
    // AttemptMove takes a generic parameter T which for Player will be of the type Plant, it also takes integers for x and y direction to move in.
    protected override void AttemptMove(int xDir, int yDir)
    {
        //Call the AttemptMove method of the base class, passing in the component T (in this case Wall) and x and y direction to move.
        base.AttemptMove(xDir, yDir);
    }
}