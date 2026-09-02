# Add the following to this file:
if app.ENABLE_TITLE_SYSTEM:
	# On/Off 옵션 - 타이틀
	window["children"][PARENT_CHILDREN_INDEX]["children"] = window["children"][PARENT_CHILDREN_INDEX]["children"] + [
				{
					"name" : "show_player_title_button",
					"type" : "toggle_button",
					
					"x" : BUTTON_TWO_RIGHT_X,
					"y" : LINE_BEGIN + LINE_STEP * (LINE_NUMBER + 5),
					"horizontal_align" : "center",

					"text" : uiScriptLocale.GAME_OPTION_PLAYER_TITLE,					# 타이틀
					
					"default_image" : GAMEOPTION_PATH + "button_128_default.sub",
					"over_image"	: GAMEOPTION_PATH + "button_128_over.sub",
					"down_image"	: GAMEOPTION_PATH + "button_128_down.sub",
				},				
		]
