using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {

    public float Velocity;
    public Rigidbody2D PlayerRB;
    private Animator anim;
    private AudioSource footSteps;
    private bool MovingRight;
    private bool MovingLeft;

    // Use this for initialization
    void Start () {
        PlayerRB = GetComponent<Rigidbody2D>();
        anim = GetComponent<Animator>();
        footSteps = GetComponent<AudioSource>();
        footSteps.Pause();
    }
	
	// Update is called once per frame
	void Update () {
        Vector3 movement = new Vector3(1.0f, 0.0f, 0.0f);
        if((Input.GetKey(KeyCode.D)) &&
           (Input.GetKey(KeyCode.A)))
        {
            anim.SetBool("IsMovingLeft", true);
            anim.SetBool("IsMovingRight", true);
            MovingRight = true;
            MovingLeft = true;
            footSteps.Pause();
        }
        else if (Input.GetKey(KeyCode.D))
        {
            /* Apply force in red arrow direction (pos x) */
            PlayerRB.velocity = (movement * Velocity);
            anim.SetBool("IsMovingRight", true);
            anim.SetBool("IsMovingLeft", false);
            MovingRight = true;
            MovingLeft = false;
            footSteps.UnPause();

        }
        else if (Input.GetKey(KeyCode.A))
        {
            /* Apply force in red arrow direction (neg x) */
            PlayerRB.velocity = ((-movement) * Velocity);
            anim.SetBool("IsMovingLeft", true);
            anim.SetBool("IsMovingRight", false);
            MovingLeft = true;
            MovingRight = false;
            footSteps.UnPause();
        }
        else
        {
            anim.SetBool("IsMovingLeft", false);
            anim.SetBool("IsMovingRight", false);
            MovingLeft = false;
            MovingRight = false;
            footSteps.Pause();
        }
    }

    public bool IsMovingRight()
    {
        return MovingRight;
    }

    public bool IsMovingLeft()
    {
        return MovingLeft;
    }
}