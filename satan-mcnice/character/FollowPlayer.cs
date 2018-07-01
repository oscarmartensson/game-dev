using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour {

    private Transform PlayerTransform;
    private Transform CameraTransform;

    // Use this for initialization
    void Start()
    {
        CameraTransform = GetComponent<Transform>();
        PlayerTransform = (GameObject.FindWithTag("Player")).GetComponent<Transform>();
    }

    // Update is called once per frame
    void Update()
    {
        float xpos = PlayerTransform.transform.position.x;
        CameraTransform.position = new Vector3(xpos, CameraTransform.position.y, CameraTransform.position.z);
    }
}
