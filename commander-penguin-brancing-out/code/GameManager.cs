using UnityEngine;
using System.Collections;
using System.Collections.Generic;                           //Allows us to use Lists. 
using UnityEngine.SceneManagement;
using UnityEngine.UI;


// --------------------------------
// Manages the game, end condition and other rules
public class GameManager : MonoBehaviour
{
    public static GameManager instance = null;              //Static instance of GameManager which allows it to be accessed by any other script.

    public int nrOfActivePlants = 0;
    public float spawnTimerForPlantsMs;
    public float spawnTimerDecreaserMs;
    public Text counterText;

    private const float spawnTimerAdjustTimerMs = 1500.0f;  // Spawn timer increased each 1.5 seconds
    private const float maxAllowedPlants        = 6 * 10 - 2;
    private float surviveCounter;
    private float plantSpawnTimer;
    private float timer;
    private BoardManager boardScript;                       //Store a reference to our BoardManager which will set up the level.

    // --------------------------------
    //Awake is always called before any Start functions
    void Awake()
    {
        //Check if instance already exists
        if (instance == null)

            //if not, set instance to this
            instance = this;

        //If instance already exists and it's not this:
        else if (instance != this)

            //Then destroy this. This enforces our singleton pattern, meaning there can only ever be one instance of a GameManager.
            Destroy(gameObject);

        //Sets this to not be destroyed when reloading scene
        DontDestroyOnLoad(gameObject);

        //Get a component reference to the attached BoardManager script
        boardScript = GetComponent<BoardManager>();

        //Call the InitGame function to initialize the first level 
        InitGame();
    }

    // --------------------------------
    // Initializes game parameters
    void InitGame()
    {
        timer = 0.0f;
        surviveCounter = 0.0f;
        plantSpawnTimer = 0;
        nrOfActivePlants = 0;
        //Call the SetupScene function of the BoardManager script, pass it current level number.
        boardScript.SetupScene();
        UpdateCountText();
    }

    // --------------------------------
    // Game over. Switch scene
    public void GameOver()
    {
        enabled = false;
        SceneManager.LoadScene("GameOver");
    }

    // --------------------------------
    // Update called each frame
    void Update()
    {
        timer += Time.deltaTime * 1000;
        plantSpawnTimer += Time.deltaTime * 1000;
        surviveCounter += Time.deltaTime;
        UpdateCountText();

        // Decrease the spawn timer for the plants each spawnTimerAdjustTimerMS miliseconds to increase difficulty
        if (timer >= spawnTimerAdjustTimerMs)
        {
            spawnTimerForPlantsMs -= spawnTimerDecreaserMs;
            timer = 0.0f;
        }

        // Spawn plant once each spawnTimerForPlantsMs miliseconds
        if (plantSpawnTimer >= spawnTimerForPlantsMs)
        {
            boardScript.SpawnPlant();
            plantSpawnTimer = 0;
        }

        // Too many plants on the board, game over
        if (nrOfActivePlants >= maxAllowedPlants)
        {
            GameOver();
        }
    }

    // --------------------------------
    // Update the text that displays number of seconds player has survived against the plants for
    void UpdateCountText()
    {
        counterText.text = "" + surviveCounter.ToString("F") + " seconds survived!";
    }
}