using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DialogueScript : MonoBehaviour {

    public float dialogueUpTime;

    public void DialogueToggle()
    {
        gameObject.SetActive(true);
        StartCoroutine(DialogueToggleCoroutine());
    }

    private IEnumerator DialogueToggleCoroutine()
    {
        yield return new WaitForSeconds(dialogueUpTime);
        gameObject.SetActive(false);
    }
}
