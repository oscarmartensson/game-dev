using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

// --------------------------------
// This script switches scene to the game scene once space is pressed
public class switchToPlaySpace : MonoBehaviour {

    private AudioSource bgMusic;

    // --------------------------------
    // Called on start
    private void Start()
    {
        bgMusic = GetComponent<AudioSource>();
        bgMusic.Play();
    }

    // --------------------------------
    // Update is called once per frame
    void Update () {
		if (Input.GetKey(KeyCode.Space))
        {
            bgMusic.Pause();
            SceneManager.LoadScene("SampleScene");
        }
	}
}
