using UnityEngine;
using System.Collections;

// --------------------------------
// Instantiates classes in the game
public class Loader : MonoBehaviour
{
    public GameObject gameManager;          //GameManager prefab to instantiate.

    // --------------------------------
    // Awake is always called before any Start functions
    void Awake()
    {
        // Check if a GameManager has already been assigned to static variable GameManager.instance or if it's still null
        if (GameManager.instance == null)
        {
            // Instantiate gameManager prefab
            Instantiate(gameManager);
        }
    }
}