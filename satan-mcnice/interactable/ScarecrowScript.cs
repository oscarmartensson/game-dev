using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class ScarecrowScript : MonoBehaviour, IPointerClickHandler, IPointerEnterHandler, IPointerExitHandler 
{

    private Color color;
    public Color mouseOverColor;
    public GameObject dialogue;

    // Use this for initialization
    void Start()
    {
        color = gameObject.GetComponent<SpriteRenderer>().color;
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        dialogue.GetComponent<DialogueScript>().DialogueToggle();
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        gameObject.GetComponent<SpriteRenderer>().color = mouseOverColor;
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        gameObject.GetComponent<SpriteRenderer>().color = color;
    }
}
