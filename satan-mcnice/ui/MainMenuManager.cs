using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenuManager : MonoBehaviour {

    public GameObject obj_1;
    public GameObject obj_2;
    public GameObject obj_3;
    public GameObject obj_4;
    public GameObject obj_5;
    public GameObject obj_6;
    public GameObject obj_7;
    public GameObject obj_8;
    public GameObject obj_9;
    public GameObject obj_10;
    public GameObject obj_11;
    public GameObject obj_12;
    public GameObject obj_13;
    public GameObject obj_14;

    public float waitTime_1 = 0f;
    public float waitTime_2 = 0f;
    public float waitTime_3 = 0f;
    public float waitTime_4 = 0f;
    public float waitTime_5 = 0f;
    public float waitTime_6 = 0f;
    public float waitTime_7 = 0f;
    public float waitTime_8 = 0f;
    public float waitTime_9 = 0f;
    public float waitTime_10 = 0f;

    public int currentState;

    public void QuitApplication()
    {
        Application.Quit();
    }

    public void LoadScene(int level)
    {
        SceneManager.LoadScene(level);
    }

    public void ToggleImage(GameObject panel)
    {
        if(panel.activeInHierarchy)
        {
            panel.SetActive(false);
        }
        else
        {
            panel.SetActive(true);
        }
    }

    public void runCinematic()
    {
        StartCoroutine(runCinematicCoroutine());
    }

    private IEnumerator runCinematicCoroutine()
    {
        currentState = 0;
        bool done = false;

        while (done == false)
        {
            switch (currentState)
            {
                case 0:
                    //Fade out main menu
                    obj_1.GetComponent<FadeImage>().FadeIn();
                    obj_2.GetComponent<FadeOutMusic>().FadeOut();
                    yield return new WaitForSeconds(waitTime_1);
                    currentState = 1;
                    break;

                case 1:
                    //Officer dialogue 1
                    obj_3.SetActive(true);
                    obj_5.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_2);
                    obj_3.SetActive(false);
                    obj_6.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_3);
                    currentState = 2;
                    break;

                case 2:
                    //Satan dialogue 1
                    obj_7.SetActive(true);
                    obj_4.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_4);
                    obj_7.SetActive(false);
                    obj_6.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_3);
                    currentState = 3;
                    break;

                case 3:
                    //Officer dialogue 2
                    obj_8.SetActive(true);
                    obj_4.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_5);
                    obj_8.SetActive(false);
                    obj_6.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_3);
                    currentState = 4;
                    break;

                case 4:
                    //Satan dialogue 2
                    obj_9.SetActive(true);
                    obj_4.GetComponent<AudioSource>().Play();
                    obj_11.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_6);
                    obj_9.SetActive(false);
                    obj_6.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_3);
                    currentState = 5;
                    break;

                case 5:
                    //Officer dialogue 3
                    obj_10.SetActive(true);
                    obj_4.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_7);
                    obj_10.GetComponent<FadeImage>().FadeOut();
                    obj_10.GetComponentInChildren<FadeText>().FadeOut();
                    yield return new WaitForSeconds(waitTime_8);
                    obj_6.GetComponent<AudioSource>().Play();
                    currentState = 6;
                    break;

                case 6:
                    obj_12.GetComponent<AudioSource>().Play();
                    yield return new WaitForSeconds(waitTime_9);
                    obj_13.GetComponent<AudioSource>().Play();
                    currentState = 7;
                    break;

                default:
                    done = true;
                    break;

            }
            yield return null;
        }
        LoadScene(1);
        yield return null;
    }
}
