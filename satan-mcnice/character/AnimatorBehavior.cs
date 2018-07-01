using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimatorBehavior : MonoBehaviour {

    private Animator anim;
    private Player player;
    public bool left;
    public bool right;

	// Use this for initialization
	void Start () {
        anim = GetComponent<Animator>();
        player = GameObject.FindWithTag("Player").GetComponent<Player>();
    }
	
	// Update is called once per frame
	void Update () {
        left = player.IsMovingLeft();
        right = player.IsMovingRight();
        if (player.IsMovingLeft())
        {
            anim.SetBool("IsMovingLeft", true);
        }
        else if (player.IsMovingRight())
        {
            anim.SetBool("IsMovingRight", true);
        }
        else
        {
            anim.SetBool("IsMovingLeft", false);
            anim.SetBool("IsMovingRight", false);
        }
	}
}
