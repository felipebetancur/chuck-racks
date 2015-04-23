//
//  ulib_pluginhost.cpp
//  chuck-vst
//
//  Created by Spencer Salazar on 4/1/15.
//
//

#include "ulib_pluginhost.h"
#include "chuck_instr.h"
#include "chuck_type.h"


PluginHostInfo::PluginHostInfo()
{
    wasPlaying=false;
    current16th=-1;
    previousBar=-1;
    
    tempo=120;
    quarterLength=60/tempo;
    eighthLength=30/tempo;
    sixteenthLength=15/tempo;
}


PluginHostInfo *g_hostInfo = NULL;


void PluginHostInfo::broadcastPlayEvent()
{
    if(g_hostInfo->playEvent != NULL)
        g_hostInfo->playEvent->broadcast();
}

void PluginHostInfo::broadcastBeatStartEvent()
{
    if(g_hostInfo->beatStartEvent != NULL)
        g_hostInfo->beatStartEvent->broadcast();
}

void PluginHostInfo::broadcastStopEvent()
{
    if(g_hostInfo->stopEvent != NULL)
        g_hostInfo->stopEvent->broadcast();
}

void PluginHostInfo::setTempo(float newTempo)
{
    tempo=newTempo;
    quarterLength=60/tempo;
    eighthLength=30/tempo;
    sixteenthLength=15/tempo;
}

void PluginHostInfo::broadcast16thHit()
{
    
    if(g_hostInfo->sixteenthEvent != NULL)
        g_hostInfo->sixteenthEvent->broadcast();

}


CK_DLL_SFUN(pluginhost_getTempo)
{
    RETURN->v_float = g_hostInfo->tempo;
}

CK_DLL_SFUN(pluginhost_setTempo)
{
    t_CKFLOAT tempo = GET_NEXT_FLOAT(ARGS);
    
    //JUCE::SetTEMPO(tempo);
    
    
    RETURN->v_float = g_hostInfo->tempo;
}

CK_DLL_SFUN(pluginhost_onPlay)
{
    if(g_hostInfo->playEvent == NULL)
    {
        g_hostInfo->playEvent = new Chuck_Event();
        initialize_object(g_hostInfo->playEvent, &t_event);
    }
    
    RETURN->v_object = g_hostInfo->playEvent;
}

CK_DLL_SFUN(pluginhost_onStop)
{
    if(g_hostInfo->stopEvent == NULL)
    {
        g_hostInfo->stopEvent = new Chuck_Event();
        initialize_object(g_hostInfo->stopEvent, &t_event);
    }
    
    RETURN->v_object = g_hostInfo->stopEvent;
}

CK_DLL_SFUN(pluginhost_onBeatStart)
{
    if(g_hostInfo->beatStartEvent == NULL)
    {
        g_hostInfo->beatStartEvent = new Chuck_Event();
        initialize_object(g_hostInfo->beatStartEvent, &t_event);
    }
    
    RETURN->v_object = g_hostInfo->beatStartEvent;
}


CK_DLL_SFUN(pluginhost_sixteenth)
{
    if(g_hostInfo->sixteenthEvent == NULL)
    {
        g_hostInfo->sixteenthEvent = new Chuck_Event();
        initialize_object(g_hostInfo->sixteenthEvent, &t_event);
    }
    
    RETURN->v_object = g_hostInfo->sixteenthEvent;
}


CK_DLL_SFUN(pluginhost_isPlaying)
{
    if (g_hostInfo->wasPlaying) {
        RETURN->v_int = 1;
    }
    else
    {
        RETURN->v_int = 0;
    }
    
}

CK_DLL_SFUN(pluginhost_sixteenthLength)
{
    RETURN->v_float = g_hostInfo->sixteenthLength;
}

CK_DLL_SFUN(pluginhost_eightLength)
{
    RETURN->v_float = g_hostInfo->eighthLength;
}

CK_DLL_SFUN(pluginhost_quarterLength)
{
    RETURN->v_float = g_hostInfo->quarterLength;
}


CK_DLL_SFUN(pluginhost_positionInBeat)
{
    RETURN->v_float = g_hostInfo->positionInBeat;
}





t_CKBOOL pluginhost_query( Chuck_DL_Query * QUERY )
{
    g_hostInfo = new PluginHostInfo;
    g_hostInfo->playEvent = NULL;
    g_hostInfo->stopEvent = NULL;
    g_hostInfo->sixteenthEvent = NULL;
    g_hostInfo->beatStartEvent = NULL;

    QUERY->begin_class(QUERY, "PluginHost", "Object");
    
    QUERY->add_sfun(QUERY, pluginhost_getTempo, "float", "getTempo"); //return type, chuck function name
    
    QUERY->add_sfun(QUERY, pluginhost_setTempo, "float", "setTempo"); //return type, chuck function name

    QUERY->add_arg(QUERY, "float", "tempo"); // first argument 
    
    QUERY->add_sfun(QUERY, pluginhost_onPlay, "Event", "onPlay"); //return type, chuck function name
    
    QUERY->add_sfun(QUERY, pluginhost_onStop, "Event", "onStop");
    
    QUERY->add_sfun(QUERY, pluginhost_onBeatStart, "Event", "onBeat");
    
    QUERY->add_sfun(QUERY, pluginhost_sixteenthLength, "float", "sixteenthLength");
    
    QUERY->add_sfun(QUERY, pluginhost_sixteenth,"Event", "sixteenth");
    
    
    QUERY->add_sfun(QUERY, pluginhost_isPlaying, "int", "isPlaying");

    QUERY->add_sfun(QUERY, pluginhost_positionInBeat, "float", "positionInBeat"); //returns a value between 0 and 0.9999 for the position in the beat. It can be used for finer subdivisions than 16ths.
    

    
    
    QUERY->end_class(QUERY);
    
    return TRUE;
}
