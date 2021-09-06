#include "config.h"

namespace TFE_DarkForces
{
	static InputBinding s_defaultKeyboardBinds[] =
	{
		// General
		{ IA_MENU_TOGGLE,      ITYPE_KEYBOARD, KEY_ESCAPE },
		{ IA_PDA_TOGGLE,       ITYPE_KEYBOARD, KEY_F1 },
		{ IA_NIGHT_VISION_TOG, ITYPE_KEYBOARD, KEY_F2 },
		{ IA_CLEATS_TOGGLE,    ITYPE_KEYBOARD, KEY_F3 },
		{ IA_GAS_MASK_TOGGLE,  ITYPE_KEYBOARD, KEY_F4 },
		{ IA_HEAD_LAMP_TOGGLE, ITYPE_KEYBOARD, KEY_F5 },
		{ IA_HEADWAVE_TOGGLE,  ITYPE_KEYBOARD, KEY_F6 },
		{ IA_HUD_TOGGLE,       ITYPE_KEYBOARD, KEY_F7 },
		{ IA_HOLSTER_WEAPON,   ITYPE_KEYBOARD, KEY_F8 },
		{ IA_AUTOMOUNT_TOGGLE, ITYPE_KEYBOARD, KEY_F8, KEYMOD_ALT },
		{ IA_CYCLEWPN_PREV,    ITYPE_KEYBOARD, KEY_F9 },
		{ IA_CYCLEWPN_NEXT,    ITYPE_KEYBOARD, KEY_F10 },
		{ IA_WPN_PREV,         ITYPE_KEYBOARD, KEY_BACKSPACE },
		{ IA_PAUSE,            ITYPE_KEYBOARD, KEY_PAUSE },
		{ IA_AUTOMAP,          ITYPE_KEYBOARD, KEY_TAB },
		{ IA_DEC_SCREENSIZE,   ITYPE_KEYBOARD, KEY_MINUS, KEYMOD_ALT },
		{ IA_INC_SCREENSIZE,   ITYPE_KEYBOARD, KEY_EQUALS, KEYMOD_ALT },

		// Automap
		// IA_MAP_ZOOM_IN,
		// IA_MAP_ZOOM_OUT,
		// IA_MAP_SCROLL_UP,
		// IA_MAP_SCROLL_DN,
		// IA_MAP_SCROLL_LT,
		// IA_MAP_SCROLL_RT,
		// IA_MAP_LAYER_UP,
		// IA_MAP_LAYER_DN,

		// Player Controls
		// IA_FORWARD,
		// IA_BACKWARD,
		// IA_STRAFE_LT,
		// IA_STRAFE_RT,
		// IA_TURN_LT,
		// IA_TURN_RT,
		// IA_LOOK_UP,
		// IA_LOOK_DN,
		// IA_CENTER_VIEW,
		// IA_RUN,
		// IA_SLOW,
		// IA_CROUCH,
		// IA_JUMP,
		// IA_USE,
		// IA_WEAPON_1,
		// IA_WEAPON_2,
		// IA_WEAPON_3,
		// IA_WEAPON_4,
		// IA_WEAPON_5,
		// IA_WEAPON_6,
		// IA_WEAPON_7,
		// IA_WEAPON_8,
		// IA_WEAPON_9,
		// IA_WEAPON_10,
		// IA_PRIMARY_FIRE,
		// IA_SECONDARY_FIRE,
	};

	static InputBinding s_defaultControllerBinds[] =
	{
		{ IA_JUMP,   ITYPE_CONTROLLER, CONTROLLER_BUTTON_A },
		{ IA_RUN,    ITYPE_CONTROLLER, CONTROLLER_BUTTON_B },
		{ IA_CROUCH, ITYPE_CONTROLLER, CONTROLLER_BUTTON_X },
		{ IA_USE,    ITYPE_CONTROLLER, CONTROLLER_BUTTON_Y },

		{ IA_MENU_TOGGLE, ITYPE_CONTROLLER, CONTROLLER_BUTTON_GUIDE },

		{ IA_PRIMARY_FIRE,   ITYPE_CONTROLLER_AXIS, AXIS_RIGHT_TRIGGER },
		{ IA_SECONDARY_FIRE, ITYPE_CONTROLLER_AXIS, AXIS_LEFT_TRIGGER },
		
		{ IA_HEAD_LAMP_TOGGLE, ITYPE_CONTROLLER, CONTROLLER_BUTTON_DPAD_RIGHT },
		{ IA_NIGHT_VISION_TOG, ITYPE_CONTROLLER, CONTROLLER_BUTTON_DPAD_LEFT },
		{ IA_AUTOMAP,          ITYPE_CONTROLLER, CONTROLLER_BUTTON_DPAD_UP },
		{ IA_GAS_MASK_TOGGLE,  ITYPE_CONTROLLER, CONTROLLER_BUTTON_DPAD_DOWN },

		{ IA_CYCLEWPN_PREV, ITYPE_CONTROLLER, CONTROLLER_BUTTON_LEFTSHOULDER },
		{ IA_CYCLEWPN_NEXT, ITYPE_CONTROLLER, CONTROLLER_BUTTON_RIGHTSHOULDER },
	};

	static InputConfig s_config = { 0 };
	static ActionState s_actions[IA_COUNT];

	void addDefaultControlBinds();

	void configStartup()
	{
		s_config.bindCount = 0;
		s_config.bindCapacity = IA_COUNT * 2;
		s_config.binds = (InputBinding*)malloc(sizeof(InputBinding)*s_config.bindCapacity);

		memset(s_actions, 0, sizeof(ActionState) * IA_COUNT);
		addDefaultControlBinds();
	}

	void configShutdown()
	{
		free(s_config.binds);
		s_config.bindCount = 0;
		s_config.bindCapacity = 0;
		s_config.binds = nullptr;
	}

	void addInputBinding(InputBinding* binding)
	{
		u32 index = s_config.bindCount;
		s_config.bindCount++;

		if (s_config.bindCount > s_config.bindCapacity)
		{
			s_config.bindCapacity += IA_COUNT;
			s_config.binds = (InputBinding*)realloc(s_config.binds, sizeof(InputBinding)*s_config.bindCapacity);
		}

		s_config.binds[index] = *binding;
	}
		
	void addDefaultControlBinds()
	{
		for (s32 i = 0; i < TFE_ARRAYSIZE(s_defaultKeyboardBinds); i++)
		{
			addInputBinding(&s_defaultKeyboardBinds[i]);
		}
		for (s32 i = 0; i < TFE_ARRAYSIZE(s_defaultControllerBinds); i++)
		{
			addInputBinding(&s_defaultControllerBinds[i]);
		}
	}

	void config_updateInput()
	{
		for (u32 i = 0; i < IA_COUNT; i++)
		{
			s_actions[i] = STATE_UP;
		}

		for (u32 i = 0; i < s_config.bindCount; i++)
		{
			InputBinding* bind = &s_config.binds[i];
			switch (bind->type)
			{
				case ITYPE_KEYBOARD:
				{
					if (TFE_Input::keyModDown(bind->keyMod))
					{
						if (TFE_Input::keyPressed(bind->keyCode))
						{
							s_actions[bind->action] = STATE_PRESSED;
						}
						else if (TFE_Input::keyDown(bind->keyCode) && s_actions[bind->action] != STATE_PRESSED)
						{
							s_actions[bind->action] = STATE_DOWN;
						}
					}
				} break;
				case ITYPE_MOUSE:
				{
					if (TFE_Input::keyModDown(bind->keyMod))
					{
						if (TFE_Input::mousePressed(bind->mouseBtn))
						{
							s_actions[bind->action] = STATE_PRESSED;
						}
						else if (TFE_Input::mouseDown(bind->mouseBtn) && s_actions[bind->action] != STATE_PRESSED)
						{
							s_actions[bind->action] = STATE_DOWN;
						}
					}
				} break;
				case ITYPE_CONTROLLER:
				{
					if (TFE_Input::buttonPressed(bind->ctrlBtn))
					{
						s_actions[bind->action] = STATE_PRESSED;
					}
					else if (TFE_Input::buttonDown(bind->ctrlBtn) && s_actions[bind->action] != STATE_PRESSED)
					{
						s_actions[bind->action] = STATE_DOWN;
					}
				} break;
			}
		}
	}
	
	ActionState getActionState(InputAction action)
	{
		return s_actions[action];
	}
}  // TFE_DarkForces