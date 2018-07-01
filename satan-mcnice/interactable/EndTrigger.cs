using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EndTrigger : MonoBehaviour {

    public float dialogue1Duration = 0.0f;
    public float dialogue2Duration = 0.0f;
    public float endTextTimer = 0.0f;
    public float EoTRadioDuration = 0.0f;
    public Transform player;
    public GameObject radioEoT;
    public GameObject radioStart;
    public GameObject dialogue_1;
    public GameObject dialogue_2;
    public GameObject endingScreen;
    private GameObject endText;
    private bool done = false;

    private void Start()
    {
        endText = endingScreen.transform.GetChild(0).gameObject;
    }

    void OnTriggerStay2D(Collider2D collision)
    {
        if ((collision.gameObject.tag == "Player") &&
            (done == false))
        {
            done = true; //Hade to add ín order to avoid last clip (radioEoT) to play over itself several times causing stutter.
            //Also fixed a number of delay issues.
            player.GetComponent<Animator>().SetBool("IsMovingRight", false);
            player.GetComponent<Animator>().SetBool("IsMovingLeft", false);
            player.GetComponent<AudioSource>().Stop();
            player.GetComponent<Player>().enabled = false;
            StartCoroutine(EndSequence(dialogue_1, dialogue_2));
        }
    }

    private IEnumerator EndSequence(GameObject dialogue_1, GameObject dialogue_2)
    {
        radioStart.GetComponent<AudioSource>().Play();
        dialogue_1.SetActive(true);
        yield return new WaitForSeconds(dialogue1Duration);
        dialogue_1.SetActive(false);
        dialogue_2.SetActive(true);
        yield return new WaitForSeconds(dialogue2Duration);
        dialogue_2.SetActive(false);
        radioEoT.GetComponent<AudioSource>().Play();
        endingScreen.GetComponent<FadeImage>().FadeIn();
        yield return new WaitForSeconds(endTextTimer);
        endText.SetActive(true);
        yield return new WaitForSeconds(6.0f);
        Application.Quit();
    }
}
