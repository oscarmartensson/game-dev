using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EndFadeOutMusic : MonoBehaviour {

    public float returnTime = 0.0f;
    public GameObject music;

    void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            AudioSource tmp = music.GetComponent<AudioSource>();
            StartCoroutine(FadeOutMusicCoroutine(tmp));
        }
    }
        

    private IEnumerator FadeOutMusicCoroutine(AudioSource music)
    {

        for (float vol = music.GetComponent<AudioSource>().volume; vol > 0.0f; vol -= 0.01f)
        {
            music.GetComponent<AudioSource>().volume = vol;
            yield return new WaitForSeconds(returnTime);
        }
        yield return null;
    }
}
