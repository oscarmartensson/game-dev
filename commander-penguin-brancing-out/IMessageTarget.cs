﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public interface IMessageTarget : IEventSystemHandler
{
    // functions that can be called via the messaging system
    void DestroyTrees();
}
