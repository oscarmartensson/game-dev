using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameController : MonoBehaviour {

    public Transform PauseMenu;
    public Transform Player;
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if(PauseMenu.gameObject.activeInHierarchy == false)
            {
                PauseMenu.gameObject.SetActive(true);
                Time.timeScale = 0.0f;
                Player.GetComponent<Player>().enabled = false;
            }
            else
            {
                ResumeGame();
            }
        }
    }
    public void ExitGame()
    {
        Application.Quit();
    }

    public void ResumeGame()
    {
        PauseMenu.gameObject.SetActive(false);
        Player.GetComponent<Player>().enabled = true;
        Time.timeScale = 1.0f;
    }
}
