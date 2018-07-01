using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeOutMusic : MonoBehaviour {

    public float returnTime = 0.0f;
    public bool done = false;

    public void FadeOut()
    {
        AudioSource music = gameObject.GetComponent<AudioSource>();
        StartCoroutine(FadeOutMusicCoroutine(music));
    }

    private IEnumerator FadeOutMusicCoroutine(AudioSource music)
    {
        
        for (float vol = music.GetComponent<AudioSource>().volume; vol > 0.0f; vol -= 0.01f)
        {
            music.GetComponent<AudioSource>().volume = vol;
            yield return new WaitForSeconds(returnTime);
        }
        yield return null;
        done = true;
    }
}
