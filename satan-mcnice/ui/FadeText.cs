using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeText : MonoBehaviour {

    public float returnTime = 0.0f;
    public bool done = false;

    public void FadeOut()
    {
        Text text = gameObject.GetComponent<Text>();
        StartCoroutine(FadeOutTextCoroutine(text));
    }

    public IEnumerator FadeOutTextCoroutine(Text text)
    {
        for (float alpha = text.color.a; alpha > 0.0f; alpha -= 0.1f)
        {
            Color tmpColor = text.GetComponent<Text>().color;
            tmpColor.a = alpha;
            text.GetComponent<Text>().color = tmpColor;
            yield return null;
        }
        yield return new WaitForSeconds(returnTime);
        done = true;
        gameObject.SetActive(false);
    }
}
