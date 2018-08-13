using UnityEngine;
using System;
using System.Collections.Generic;       // Allows us to use Lists.
using Random = UnityEngine.Random;      // Tells Random to use the Unity Engine random number generator.
using UnityEngine.EventSystems;


// --------------------------------
// Manages the play board
public class BoardManager : MonoBehaviour
{
    public GameObject player;

    [Serializable]
    public class Count
    {
        public int minimum;             // Minimum value for our Count class.
        public int maximum;             // Maximum value for our Count class.

        //Assignment constructor.
        public Count(int min, int max)
        {
            minimum = min;
            maximum = max;
        }
    }

    public int columns = 10;                                        // Number of columns in our game board.
    public int rows = 6;                                            // Number of rows in our game board.
    public Count plantCount = new Count(5, 9);                      // Lower and upper limit for our random number of food items per level.
    public GameObject[] plantTiles;                                 // Array of enemy prefabs.
    public Transform boardHolder;                                   // A variable to store a reference to the transform of our Board object.

    private List<Vector3> gridPositions = new List<Vector3>();      // A list of possible locations to place tiles


    // --------------------------------
    // Clears our list gridPositions and prepares it to generate a new board.
    void InitialiseList()
    {
        // Clear our list gridPositions.
        gridPositions.Clear();

        // Loop through x axis (columns).
        for (int x = 0; x < columns; x++)
        {
            // Within each column, loop through y axis (rows).
            for (int y = 0; y < rows; y++)
            {
                // At each index add a new Vector3 to our list with the x and y coordinates of that position.
                gridPositions.Add(new Vector3(x, y, 0f));
            }
        }
    }

    // --------------------------------
    // LayoutObjectAtRandom accepts an array of game objects to choose from along with a minimum and maximum range for the number of objects to create.
    void LayoutObjectAtRandom(GameObject[] tileArray, int minimum, int maximum)
    {
        // Choose a random number of objects to instantiate within the minimum and maximum limits
        int objectCount = Random.Range(minimum, maximum + 1);

        // Instantiate objects until the randomly chosen limit objectCount is reached
        int i = 0;
        while (i < objectCount)
        {
            // Declare an integer randomIndex, set it's value to a random number between 0 and the count of items in our List gridPositions.
            int randomIndex = Random.Range(0, gridPositions.Count);

            // Declare a variable of type Vector3 called randomPosition, set it's value to the entry at randomIndex from our List gridPositions.
            Vector3 randomPosition = gridPositions[randomIndex];

            if (Vector3.Distance(randomPosition, player.transform.position) < float.Epsilon)
            {
                // We're trying to put it on the player. Not good.
                continue;
            }

            // Choose a random tile from tileArray and assign it to tileChoice
            GameObject tileChoice = tileArray[Random.Range(0, tileArray.Length)];

            // Instantiate tileChoice at the position returned by RandomPosition with no change in rotation
            GameObject instance = Instantiate(tileChoice, randomPosition, Quaternion.identity);
            instance.transform.SetParent(boardHolder);

            if (instance.tag == "Plant")
            {
                GameObject.Find("GameManager").GetComponent<GameManager>().nrOfActivePlants++;
            }

            i++;
        }
    }


    // --------------------------------
    // SetupScene initializes our level and calls the previous functions to lay out the game board
    public void SetupScene()
    {
        // Creates the outer walls and floor.
        boardHolder = new GameObject("Board").transform;

        //Reset our list of gridpositions.
        InitialiseList();

        //Instantiate a random number of plants based on minimum and maximum, at randomized positions.
        LayoutObjectAtRandom(plantTiles, plantCount.minimum, plantCount.maximum);

        return;
    }


    // --------------------------------
    // Spawns a plant on the board
    public void SpawnPlant()
    {
        // Declare an integer randomIndex, set it's value to a random number between 0 and the count of items in our List gridPositions.
        int randomIndex = Random.Range(0, gridPositions.Count);

        // Declare a variable of type Vector3 called randomPosition, set it's value to the entry at randomIndex from our List gridPositions.
        Vector3 randomPosition = gridPositions[randomIndex];

        // Choose a random tile from tileArray and assign it to tileChoice
        GameObject tileChoice = plantTiles[Random.Range(0, plantTiles.Length)];

        float distToPlayer = Vector3.Distance(player.transform.position, randomPosition);

        if (BoardCollision(distToPlayer, randomPosition) == true)
        {
            // Spawning of plant either collides with player or existing plant. Recall function with new random position
            SpawnPlant();
        }

        else
        {
            // Ok to spawn tree, no collisions
            GameObject instance = Instantiate(tileChoice, randomPosition, Quaternion.identity);
            instance.transform.SetParent(boardHolder);
            GameObject.Find("GameManager").GetComponent<GameManager>().nrOfActivePlants++;
            // Remove the entry at randomIndex from the list so that it can't be re-used.
            gridPositions.RemoveAt(randomIndex);
        }

        return;
    }


    // --------------------------------
    // Checks the board for either player or existing plant collisions
    private bool BoardCollision(float distToPlayer, Vector3 randomPosition)
    {
        bool collision = false;

        if (distToPlayer <= float.Epsilon)
        {
            //Trying to spawn new tree on player
            collision = true;
            return collision;
        }

        foreach (Transform child in boardHolder)
        {
            // Make sure we don't colide with spawned plant
            float distanceToPlant = Vector3.Distance(child.position, randomPosition); 
            if (distanceToPlant <= float.Epsilon)
            {
                collision = true;
                return collision;
            }
        }

        return collision;
    }


    // --------------------------------
    // Destroy trees adjacent to unitPosition (if there are any)
    public void DestroyTrees(Vector3 unitPosition)
    {
        foreach (Transform child in boardHolder)
        {
            float distanceToPlant = Vector3.Distance(child.position, unitPosition);
            if ((distanceToPlant - 1.0f) < 0.001f || (distanceToPlant - 1.414214) < 0.01f)
            {
                Debug.Log("Removed plant at " + child.position.ToString());
                // Plant is nearby, remove it
                int activePlants = GameObject.Find("GameManager").GetComponent<GameManager>().nrOfActivePlants;
                if (activePlants > 0)
                {
                    GameManager gameManager = GameObject.Find("GameManager").GetComponent<GameManager>();
                    gameManager.nrOfActivePlants--;
                    GameManager.destroyedTrees++; // Note static variable
                }

                // Add poisítion to list and remove object
                gridPositions.Add(child.transform.position);
                GameObject.Destroy(child.gameObject);
            }
        }

        return;
    }
}