using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeImage : MonoBehaviour {

    public float    returnTime  = 0.0f;
    public bool     done        = false;

    public void FadeIn()
    {
        gameObject.SetActive(true);
        Image image = gameObject.GetComponent<Image>();
        StartCoroutine(FadeInImageCoroutine(image));
    }

    public IEnumerator FadeInImageCoroutine(Image image)
    {
        for (float alpha = 0.0f; alpha < 1.0f; alpha += 0.01f)
        {
            Color tmpColor = image.GetComponent<Image>().color;
            tmpColor.a = alpha;
            image.GetComponent<Image>().color = tmpColor;
            yield return new WaitForSeconds(returnTime);
        }
        yield return null; ;
        done = true;
    }

    public void FadeOut()
    {
        Image image = gameObject.GetComponent<Image>();
        StartCoroutine(FadeOutImageCoroutine(image));
    }

    public IEnumerator FadeOutImageCoroutine(Image image)
    {
        for (float alpha = image.color.a; alpha > 0.0f; alpha -= 0.01f)
        {
            Color tmpColor = image.GetComponent<Image>().color;
            tmpColor.a = alpha;
            image.GetComponent<Image>().color = tmpColor;
            yield return null;
        }
        yield return new WaitForSeconds(returnTime);
        done = true;
        gameObject.SetActive(false);
    }
}
