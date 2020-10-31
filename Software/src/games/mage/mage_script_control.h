/*
This class contains all the code related to the hex editor hacking interface.
*/
#ifndef _MAGE_SCRIPT_CONTROL_H
#define _MAGE_SCRIPT_CONTROL_H

#include "mage_defines.h"
#include "mage.h"
#include "mage_game_control.h"
#include "mage_hex.h"

//this is a class designed to handle all the scripting for the MAGE() game
//it is designed to work in tandem with a MageGameControl object and a
//MageHex object to effect that state of the game.

class MageScriptControl
{
	private:
		//the jumpScript variable is used by some actions to indicate that a script should
		//end and immediately begin running a new script.
		//it should be set to MAGE_NULL_SCRIPT unless a new script should be run immediately.
		uint16_t jumpScript;

		//this tracks whether or not an action has been called that will require an update
		//and render to be processed before any blocking actions or when the script is completed.
		bool scriptRequiresRender;

		//variables for tracking suspended script states:
		MageScriptState mapLoadResumeState;
		MageScriptState mapTickResumeState;
		MageScriptState entityInteractResumeStates[MAX_ENTITIES_PER_MAP];
		MageScriptState entityTickResumeStates[MAX_ENTITIES_PER_MAP];

		//typedef for the array of function pointers to script action functions:
		typedef void(MageScriptControl::*ActionFunctionPointer)(uint8_t * args, MageScriptState * resumeStateStruct);

		//the actual array of action functions:
		ActionFunctionPointer actionFunctions[MageScriptActionTypeId::NUM_ACTIONS];

		//this resets the values of a MageScriptState struct to default values.
		//you need to provide a scriptId, and the state of the scriptIsRunning variable
		//the actionId, and duration variables are always reset to 0 on an init.
		void initScriptState(MageScriptState * resumeStateStruct, uint16_t scriptId, bool scriptIsRunning);

		//this will process a script based on the state of the resumeStateStruct passed to it.
		//it should only be called from the 
		void processScript(MageScriptState * resumeStateStruct);

		//this will run through the actions in a script from the state stores in resumeState
		//if a jumpScript is called by an action, it will return without processing any further actions.
		void processActionQueue(MageScriptState * resumeStateStruct);

		//this will get action arguments from ROM memory and call
		//a function based on the ActionTypeId 
		void runAction(uint32_t argumentMemoryAddress, MageScriptState * resumeStateStruct);

		//the functions below here are the action functions. These are going to be
		//called directly by scripts, and preform their actions based on arguments read from ROM

		//the following four types of actions describe how the actions will be run within the program:
		//I   = instant, will execute and immediately proceed to the next action
		//NB  = non-blocking, will use loopsToNextAction and totalLoopsToNextAction to run the action until it is completed
		//NBC = non-blocking continuous, will never proceed to another action, and will begin the same action again forever until the scriptId is changed
		//B   = blocking, will pause all game actions until complete.
		//I+U = Scripts that are applied to the game state instantly but will require a new update and render before the changes take effect
		//I+C = scripts that may call another scriptId, discarding any actions that occur after them in the current script
		//I've noted the blocking state of actions below on the line above the action:

		//I
		void nullAction(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void checkEntityByte(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void checkSaveFlag(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void checkIfEntityIsInGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void checkForButtonPress(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void checkForButtonState(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void runScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+C
		void compareEntityName(uint8_t * args, MageScriptState * resumeStateStruct);
		//B
		void blockingDelay(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void nonBlockingDelay(uint8_t * args, MageScriptState * resumeStateStruct);
		//B (note, setPauseState require a specific hard-coded key press to unpause the game, pause state can be activated by scripts but only deactivated by player action)
		void setPauseState(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setEntityByte(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setSaveFlag(uint8_t * args, MageScriptState * resumeStateStruct);
		//I
		void setPlayerControl(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setEntityInteractScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setEntityTickScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setMapTickScript(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setEntityType(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setHexCursorLocation(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setHexBit(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void unlockHaxCell(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void lockHaxCell(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U (loadMap will stop all other scripts immediately, loading a new map with new scripts)
		void loadMap(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void screenShake(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void screenFadeOut(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void screenFadeIn(uint8_t * args, MageScriptState * resumeStateStruct);
		//B (note showDialog will pause the main game loop and run a dialog Loop until the dialog is concluded. No other actions in the game will occur while a dialog window is active)
		void showDialog(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setRenderableFont(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void moveEntityToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void moveEntityAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//NBC
		void loopEntityAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void moveCameraToGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB
		void moveCameraAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//NBC
		void loopCameraAlongGeometry(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setEntityDirection(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setHexEditorState(uint8_t * args, MageScriptState * resumeStateStruct);
		//I+U
		void setHexEditorDialogMode(uint8_t * args, MageScriptState * resumeStateStruct);
		//NB (sounds should begin playing when called by an action and continue until the sound file ends)
		void playSound(uint8_t * args, MageScriptState * resumeStateStruct);
	public:
		MageScriptControl();

		//returns size in RAM of all reserved class variables.
		uint32_t size() const;

		//these functions return the specified MageScriptState struct:
		MageScriptState getMapLoadResumeState();
		MageScriptState getMapTickResumeState();
		MageScriptState getEntityInteractResumeState(uint8_t index);
		MageScriptState getEntityTickResumeState(uint8_t index);

		//these functions will call the appropriate script processing for their script type:
		void handleMapOnLoadScript();
		void handleMapOnTickScript();
		void handleEntityOnInteractScript(uint8_t index);
		void handleEntityOnTickScript(uint8_t index);

}; //MageScriptControl

#endif //_MAGE_SCRIPT_CONTROL_H
