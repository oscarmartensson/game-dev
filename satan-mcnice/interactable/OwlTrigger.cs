using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OwlTrigger : MonoBehaviour {

    public GameObject owl;
    public GameObject dialogue;
    public float delay = 0.0f;
    public float dialogueDuration = 4.0f;
    private bool Hooed;

    private void Start()
    {
        Hooed = false;
    }

    void OnTriggerStay2D(Collider2D collision)
    {
        if ((collision.gameObject.tag == "Player") &&
            (Hooed == false))
        {
            owl.GetComponent<AudioSource>().Play();
            Hooed = true;
            StartCoroutine(OwlDialogue());
        }
    }

    private IEnumerator OwlDialogue()
    {
        yield return new WaitForSeconds(delay);
        dialogue.SetActive(true);
        yield return new WaitForSeconds(dialogueDuration);
        dialogue.SetActive(false);
    }
}
