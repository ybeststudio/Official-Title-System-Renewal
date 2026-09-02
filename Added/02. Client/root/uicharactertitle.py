import app
import ui
import localeInfo
import uiScriptLocale
import titleSystem
import chat
import math

try:
	import dbg
except:
	dbg = None

try:
	import wndMgr
except:
	wndMgr = None

try:
	import uiToolTip
except:
	uiToolTip = None

def GetTitleDetails(titleIndex):
	try:
		import localeInfo
		locale_path = app.GetLocalePath()
		locale_name = locale_path.replace("\\", "/").split("/")[-1]
		paths = [
			locale_path + "/title_gf.txt",
			locale_path + "/title_wz.txt",
			"locale/locale/%s/title_gf.txt" % locale_name,
			"locale/%s/title_gf.txt" % locale_name,
			"locale/locale/%s/title_wz.txt" % locale_name,
			"locale/%s/title_wz.txt" % locale_name,
			"locale/locale/tr/title_gf.txt",
			"locale/tr/title_gf.txt",
			"locale/locale/tr/title_wz.txt",
			"locale/tr/title_wz.txt"
		]
		for p in paths:
			if app.IsExistFile(p):
				try:
					handle = app.OpenTextFile(p)
					if handle > 0:
						lineCount = app.GetTextFileLineCount(handle)
						for i in xrange(lineCount):
							line = app.GetTextFileLine(handle, i).strip()
							if not line or line.startswith("#"):
								continue
							parts = line.split("\t")
							if len(parts) >= 5:
								idx = int(parts[0])
								if idx == titleIndex:
									name = parts[3].strip()
									condition = parts[4].strip()
									desc = parts[5].strip() if len(parts) >= 6 else ""
									
									duration = 0
									is_permanent = 1
									if titleIndex >= 1001 and titleIndex <= 1004:
										is_permanent = 0
									
									if "title_wz" in p and len(parts) >= 9:
										try:
											duration = int(parts[8])
											if len(parts) >= 10:
												is_permanent = 0 if int(parts[9]) == 1 else 1
										except:
											pass
											
									return {
										"name": name,
										"condition": condition,
										"description": desc,
										"duration": duration,
										"is_permanent": is_permanent
									}
				except:
					pass
	except:
		pass
	
	is_perm = 0 if (titleIndex >= 1001 and titleIndex <= 1004) else 1
	return {
		"name": "",
		"condition": "",
		"description": "",
		"duration": 0,
		"is_permanent": is_perm
	}

def GetTitleDescription(titleIndex):
	details = GetTitleDetails(titleIndex)
	return details["description"]

def GetTitleConditionText(titleIndex):
	details = GetTitleDetails(titleIndex)
	return details["condition"]

class TitleInfo(object):
	def __init__(self, row):
		self.__index = int(row[titleSystem.DATA_COLUMN_TITLE_INDEX])
		self.__type = int(row[titleSystem.DATA_COLUMN_TITLE_TYPE])
		self.__name = row[titleSystem.DATA_COLUMN_NAME]
		self.__tooltip_text = row[titleSystem.DATA_COLUMN_CONDITION_TOOLTIP]
		self.__is_permanent = int(row[titleSystem.DATA_COLUMN_IS_PERMANENT])
		self.__open_time = int(row[titleSystem.DATA_COLUMN_OPEN_TIME])
		self.__font_color = int(row[titleSystem.DATA_COLUMN_FONT_COLOR])
		self.__resource_index = int(row[titleSystem.DATA_COLUMN_RESOURCE_INDEX])

	def GetIndex(self):
		return self.__index

	def GetName(self):
		return self.__name

	def GetType(self):
		return self.__type

	def GetResourceIndex(self):
		return self.__resource_index

	def GetFontColor(self):
		return self.__font_color

	def IsPermanent(self):
		return self.__is_permanent != 0

	def GetConditionText(self):
		val = GetTitleConditionText(self.__index)
		if val:
			return val
		if isinstance(self.__tooltip_text, str):
			return self.__tooltip_text
		return ""

	def GetToolTipText(self):
		return self.GetConditionText()


class PlayerTitleInfo(object):
	def __init__(self, row):
		self.__index = int(row[titleSystem.PLAYER_COLUMN_TITLE_INDEX])
		self.__end_time = int(row[titleSystem.PLAYER_COLUMN_END_TIME])
		self.__is_equip = int(row[titleSystem.PLAYER_COLUMN_IS_EQUIP])
		self.__is_obtain = int(row[titleSystem.PLAYER_COLUMN_IS_OBTAIN])

	def __lt__(self, other):
		return self.__index < other.__index

	def GetIndex(self):
		return self.__index

	def GetEndTime(self):
		return self.__end_time

	def IsEquipped(self):
		return self.__is_equip != 0

	def IsObtained(self):
		return self.__is_obtain != 0


class CharacterTitleWindow(ui.ScriptWindow):
	DEBUG_LOG = False
	DEBUG_HOVER_PROBE = False
	DEBUG_RT_POLL = False
	LIST_UI_VERSION = 6
	SHOW_BUTTON_MAX = 9
	SCROLL_SIZE = 3
	LIST_BOARD_X = 12
	LIST_BOARD_Y = 32
	LIST_BUTTON_X = 4
	LIST_BUTTON_WIDTH = 200
	LIST_BUTTON_HEIGHT = 23
	LIST_START_Y = 24
	ROOT_PATH = "d:/ymir work/ui/game/title/window/"
	BUTTON_DEFAULT = ROOT_PATH + "list_button_default.sub"
	BUTTON_OVER = ROOT_PATH + "list_button_over.sub"
	BUTTON_DOWN = ROOT_PATH + "list_button_down.sub"
	BUTTON_DEFAULT_DISABLE = ROOT_PATH + "list_button_default_disable.sub"
	BUTTON_DISABLE = ROOT_PATH + "list_button_disable.sub"
	BUTTON_DEFAULT_EQUIPPED = ROOT_PATH + "list_button_default_equipped.sub"
	STATE_LOCKED = ROOT_PATH + "state_locked.sub"
	STATE_HOURGLASS = ROOT_PATH + "state_hourglass.sub"
	STATE_PERMANENT = ROOT_PATH + "state_infinite.sub"
	FONT_COLOR_GRAY = -7434610
	TITLE_SUB_PATH = "d:/ymir work/ui/game/title/titles/title_%04d.sub"
	YMIR_ROOT = "d:/ymir work/"
	RIGHT_PANEL_WIDTH = 224
	PREVIEW_BANNER_Y = 155
	PREVIEW_PLACEHOLDER_TEXT_COLOR = 0xFFFFFFFF
	NAMEPLATE_MIDDLE_PAD_X = 30
	NAMEPLATE_STATIC_PREVIEW_SCALE = 0.75
	NAMEPLATE_STATIC_PREVIEW_SCALE_Y = 1.30	#Y ekseninde scale
	NAMEPLATE_TOTAL_WIDTH = 208
	SPRITE_FRAME_COUNT = 36
	SELECTED_NAME_Y = 15
	MAX_TEXT_WIDTH = 145
	MOVING_DIRECTION = -1
	PREVIEW_RESOURCE_BY_TITLE = {
	}
	# Sunucu/proto kaynakli kesin onizleme (pack .sub type yanlis olsa bile).
	NATIVE_EFFECT_TITLE_IDS = (3005, 3006)
	NAMEPLATE_BANNER_DDS = {
		"title_06_gold": "ui/game/title/title_06_banner_gold.dds",
		"title_07_red": "ui/game/title/title_07_banner_red.dds",
		"title_08_blue": "ui/game/title/title_08_banner_blue.dds",
	}
	NAMEPLATE_TGA = {
		"gold": (
			"ui/game/title/title_06_gold_left.tga",
			"ui/game/title/title_06_gold_middle.tga",
			"ui/game/title/title_06_gold_right.tga",
		),
		"red": (
			"ui/game/title/title_07_red_left.tga",
			"ui/game/title/title_07_red_middle.tga",
			"ui/game/title/title_07_red_right.tga",
		),
		"blue": (
			"ui/game/title/title_08_blue_left.tga",
			"ui/game/title/title_08_blue_middle.tga",
			"ui/game/title/title_08_blue_right.tga",
		),
	}
	# .mse onizleme: yalnizca sprite veya statik nameplate (parca DDS animasyonu yok).
	EFFECT_PREVIEW_SPECS = {
		"title_06_banner_gold.mse": ("sprite", 1000),
		"title_07_banner_red.mse": ("sprite", 1001),
		"title_08_banner_blue.mse": ("sprite", 1002),
		"title_05_medal.mse": ("sprite", 1000),
		"title_04_trophy.mse": ("image", "effect/battleroyale/crown01.tga"),
		"title_03_fist.mse": ("nameplate", "gold"),
		"title_02_dragon.mse": ("nameplate", "red"),
		"title_01_shield.mse": ("native", None),
		"title_dragon_red.mse": ("native", None),
		"title_dragon_gw.mse": ("native", None),
	}
	DRAGON_PREVIEW_LAYERS = {
		3005: (
			("effect/etc/title/title_ring.dds", 48, 80, 1.00, 0.92, 0.00, 0.34, "ring"),
			("effect/etc/title/title_dra_red04.dds", 48, 70, 1.00, 0.78, 0.52, 0.34, "dragon"),
			("effect/etc/title/title_dra_red03.dds", 32, 88, 1.00, 1.00, 0.58, 0.36, "dragon"),
			("effect/etc/title/title_dra_red01.dds", 48, 104, 1.00, 0.88, 0.66, 0.30, "dragon"),
			("effect/etc/title/title_dra_red02.dds", 48, 96, 1.00, 0.72, 0.72, 0.28, "dragon"),
		),
		3006: (
			("effect/etc/title/title_ring_blue.dds", 48, 80, 1.00, 0.92, 0.00, 0.34, "ring"),
			("effect/etc/title/title_dra_ball.dds", 48, 78, 0.92, 0.76, 0.22, 0.34, "ring"),
			("effect/etc/title/title_dra_gw04.dds", 48, 70, 1.00, 0.78, 0.52, 0.34, "dragon"),
			("effect/etc/title/title_dra_gw03.dds", 32, 88, 1.00, 1.00, 0.58, 0.36, "dragon"),
			("effect/etc/title/title_dra_gw01.dds", 48, 104, 1.00, 0.88, 0.66, 0.30, "dragon"),
			("effect/etc/title/title_dra_gw02.dds", 48, 96, 1.00, 0.72, 0.72, 0.28, "dragon"),
		),
	}
	# C++ GetDefaultEffectPath ile uyumlu; sub'da effect yoksa fallback adi.
	TITLE_DEFAULT_EFFECT = {
		2001: "title_03_fist.mse",
		2002: "title_02_dragon.mse",
		2003: "title_01_shield.mse",
		3005: "title_dragon_red.mse",
		3006: "title_dragon_gw.mse",
		1001: "title_01_shield.mse",
		1005: "title_06_banner_gold.mse",
		1006: "title_08_banner_blue.mse",
		1011: "title_07_banner_red.mse",
	}
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.isLoaded = False
		self.__interface = None
		self.__main_board = None
		self.__list_board = None
		self.__right_board = None
		self.__scroll_bar = None
		self.__equipped_name = None
		self.__apply_button = None
		self.__clear_button = None
		self.__back_button = None
		self.__render_target = None
		self.__preview_bg_box = None
		self.__preview_uses_native_rt = False
		self.__selected_title_name = None
		self.__sub_dict = {}
		self.__preview_widgets = []
		self.__dragon_preview_layers = []
		self.__dragon_preview_start_time = 0.0
		self.__selected_name_scroll = 0
		self.__diff = 0
		self.__tooltip = None
		self.__title_dict = {}
		self.__title_button_list = []
		self.__show_title_list = []
		self.__select_button_index = -1
		self.__scroll_pos = 0
		self.__title_cnt = 0
		self.__list_ui_version = 0
		self.__debug_tick = 0
		self.__debug_last_hover_slot = -2
		self.__debug_rt_tick = 0
		self.__debug_last_rt_key = ""
		self.__debug_last_preview_key = ""

	def __DebugLog(self, msg):
		if not self.DEBUG_LOG:
			return
		text = "[TitleUI] " + str(msg)
		try:
			chat.AppendChat(chat.CHAT_TYPE_INFO, text)
		except:
			pass
		if dbg:
			try:
				dbg.TraceError(text)
			except:
				pass

	def __DebugPreviewTrace(self, step, detail=""):
		if not self.DEBUG_LOG:
			return
		text = "preview.%s" % step
		if detail:
			text += " " + str(detail)
		self.__DebugLog(text)

	def __DebugDumpCppPreview(self, tag):
		if not self.DEBUG_LOG:
			return
		try:
			dbgTuple = titleSystem.GetPreviewDebug()
		except:
			self.__DebugLog("%s cpp=GetPreviewDebug MISSING (rebuild client)" % tag)
			return
		try:
			show, res, titleIdx, inst, crc, hasBg, hasRt, rectOk, l, t, r, b, lastErr, skip, path = dbgTuple
		except:
			self.__DebugLog("%s cpp=GetPreviewDebug bad tuple" % tag)
			return
		self.__DebugLog(
			"%s cpp show=%d res=%d title=%d inst=0x%X crc=0x%X bg=%d rt=%d rect=%d (%d,%d,%d,%d) err=%d skip=%d path=%s" % (
				tag,
				int(show),
				int(res),
				int(titleIdx),
				int(inst),
				int(crc),
				int(hasBg),
				int(hasRt),
				int(rectOk),
				int(l),
				int(t),
				int(r),
				int(b),
				int(lastErr),
				int(skip),
				path,
			)
		)

	def __DebugDumpRtWidget(self, tag):
		if not self.DEBUG_LOG:
			return
		if not self.__render_target:
			self.__DebugLog("%s rt=missing widget" % tag)
			return
		try:
			show = int(self.__render_target.IsShow())
			x, y = self.__render_target.GetLocalPosition()
			w = self.__render_target.GetWidth()
			h = self.__render_target.GetHeight()
			gx, gy = self.__render_target.GetGlobalPosition()
			rtIndex = getattr(self.__render_target, "number", -1)
		except:
			self.__DebugLog("%s rt=read failed" % tag)
			return
		self.__DebugLog(
			"%s rt index=%s show=%d local=(%d,%d) size=(%d,%d) global=(%d,%d)" % (
				tag,
				rtIndex,
				show,
				x,
				y,
				w,
				h,
				gx,
				gy,
			)
		)

	def __DebugDumpPreviewWidgets(self, tag):
		if not self.DEBUG_LOG:
			return
		self.__DebugLog("%s widgets=%d" % (tag, len(self.__preview_widgets)))
		for i, widget in enumerate(self.__preview_widgets):
			try:
				show = int(widget.IsShow())
				x, y = widget.GetLocalPosition()
				w = widget.GetWidth()
				h = widget.GetHeight()
				gx, gy = widget.GetGlobalPosition()
				self.__DebugLog(
					"  w[%d] show=%d local=(%d,%d) size=(%d,%d) global=(%d,%d) cls=%s" % (
						i,
						show,
						x,
						y,
						w,
						h,
						gx,
						gy,
						widget.__class__.__name__,
					)
				)
			except:
				self.__DebugLog("  w[%d] read failed" % i)

	def __DebugDumpEnv(self, tag):
		if not self.DEBUG_LOG:
			return
		try:
			renderTarget = int(app.RENDER_TARGET)
		except:
			renderTarget = -1
		try:
			titleSys = int(app.ENABLE_TITLE_SYSTEM)
		except:
			titleSys = -1
		try:
			rtTitleIndex = int(app.RENDER_TARGET_INDEX_TITLE)
		except:
			rtTitleIndex = -1
		self.__DebugLog(
			"%s env RENDER_TARGET=%d ENABLE_TITLE_SYSTEM=%d RT_INDEX_TITLE=%d" % (
				tag,
				renderTarget,
				titleSys,
				rtTitleIndex,
			)
		)

	def Destroy(self):
		titleSystem.SetShow(0)
		titleSystem.SetHandler(None)
		self.__ClearPreviewWidgets()
		self.__ClearPreviewResource()
		self.Hide()
		self.ClearDictionary()
		self.__Initialize()

	def BindInterface(self, interface):
		self.__interface = interface

	def Open(self, open_with_selected=False, title_index=0):
		titleSystem.SetShow(1)
		titleSystem.SetHandler(self)
		if not self.isLoaded:
			self.__LoadWindow()
		elif self.__list_ui_version != self.LIST_UI_VERSION:
			self.__RebuildListButtons()
		else:
			self.__SyncListButtonPositions()
		titleSystem.RequestOpen()
		ui.ScriptWindow.Show(self)
		self.SetTop()
		self.Refresh()
		self.__DebugDumpEnv("Open")
		self.__DebugDumpRtWidget("Open")
		self.__DebugLogState("Open")
		if open_with_selected and title_index:
			self.__SelectByTitleIndex(title_index)

	def OnUpdate(self):
		if self.__selected_title_name and self.IsShow():
			self.__UpdateSelectedNameScroll()
		if self.IsShow():
			self.__UpdateDragonPreview()
		if self.DEBUG_LOG and self.DEBUG_RT_POLL and self.IsShow():
			self.__debug_rt_tick += 1
			if (self.__debug_rt_tick % 90) == 0:
				self.__DebugDumpRtWidget("poll")
				self.__DebugDumpCppPreview("poll")
		if not self.DEBUG_LOG or not self.DEBUG_HOVER_PROBE or not self.IsShow():
			return
		self.__debug_tick += 1
		if (self.__debug_tick % 45) != 0:
			return
		hoverSlot = self.__ProbeMouseListSlot()
		if hoverSlot == self.__debug_last_hover_slot:
			return
		self.__debug_last_hover_slot = hoverSlot
		if hoverSlot < 0:
			self.__DebugLog("hover: none")
		else:
			self.__DebugLog("hover probe slot=%d" % hoverSlot)

	def Close(self):
		titleSystem.SetShow(0)
		self.__ClearPreviewWidgets()
		self.__ClearPreviewResource()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnMouseWheel(self, nLen):
		if nLen > 0:
			return self.OnMouseWheelButtonUp()
		if nLen < 0:
			return self.OnMouseWheelButtonDown()
		return False

	def OnMouseWheelButtonUp(self):
		if self.__scroll_bar and self.__scroll_bar.IsShow():
			pos = max(0.0, self.__scroll_bar.GetPos() - 0.1)
			self.__scroll_bar.SetPos(pos)
			self.__OnScroll()
			return True
		return False

	def OnMouseWheelButtonDown(self):
		if self.__scroll_bar and self.__scroll_bar.IsShow():
			pos = min(1.0, self.__scroll_bar.GetPos() + 0.1)
			self.__scroll_bar.SetPos(pos)
			self.__OnScroll()
			return True
		return False

	def __LoadWindow(self):
		if self.isLoaded:
			return

		try:
			if localeInfo.IsARABIC() or localeInfo.IsVIETNAM() or localeInfo.IsJAPAN():
				self.__LoadScript(uiScriptLocale.LOCALE_UISCRIPT_PATH + "CharacterTitleWindow.py")
			else:
				self.__LoadScript("UIScript/charactertitlewindow.py")
		except:
			import exception
			exception.Abort("CharacterTitleWindow.__LoadWindow")

		self.__main_board = self.GetChild("main_board")
		self.__list_board = self.GetChild("list_board")
		self.__right_board = self.GetChild("right_board")
		self.__equipped_name = self.GetChild("equipped_name")
		self.__apply_button = self.GetChild("apply_button")
		self.__clear_button = self.GetChild("clear_button")
		self.__back_button = self.GetChild("back_button")
		self.__scroll_bar = self.GetChild("scroll_bar")
		self.__render_target = self.GetChild("model_render_target")

		self.__apply_button.SetEvent(ui.__mem_func__(self.__RequestEquip))
		self.__clear_button.SetEvent(ui.__mem_func__(self.__RequestUnEquip))
		self.__back_button.SetEvent(ui.__mem_func__(self.__ClickBackButton))
		self.__back_button.SetOverEvent(ui.__mem_func__(self.__OverInBackButton))
		self.__back_button.SetOverOutEvent(ui.__mem_func__(self.__OverOutBackButton))
		self.GetChild("main_title_bar").SetCloseEvent(ui.__mem_func__(self.Close))
		self.__scroll_bar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))

		try:
			listBar = self.GetChild("list_bar")
			listBar.AddFlag("not_pick")
		except:
			pass

		self.__tooltip = None
		if uiToolTip:
			try:
				self.__tooltip = uiToolTip.ToolTip()
				self.__tooltip.Hide()
			except:
				self.__tooltip = None

		self.__SetupRightPanel()
		self.__BuildButtons()
		self.__list_ui_version = self.LIST_UI_VERSION
		self.isLoaded = True

	def __LoadScript(self, fileName):
		loader = ui.PythonScriptLoader()
		loader.LoadScriptFile(self, fileName)

	def __GetListButtonLayout(self):
		startY = self.LIST_START_Y
		areaHeight = 235
		if self.__scroll_bar:
			try:
				_, startY = self.__scroll_bar.GetLocalPosition()
				areaHeight = self.__scroll_bar.GetHeight()
			except:
				pass
		if self.SHOW_BUTTON_MAX <= 1:
			stepY = self.LIST_BUTTON_HEIGHT
		else:
			stepY = (areaHeight - self.LIST_BUTTON_HEIGHT) // (self.SHOW_BUTTON_MAX - 1)
			if stepY < self.LIST_BUTTON_HEIGHT:
				stepY = self.LIST_BUTTON_HEIGHT
		return self.LIST_BUTTON_X, startY, stepY

	def __RebuildListButtons(self):
		for entry in self.__title_button_list:
			try:
				entry["button"].Hide()
			except:
				pass
		self.__title_button_list = []
		self.__BuildButtons()
		self.__list_ui_version = self.LIST_UI_VERSION

	def __GetListRowPosition(self, slotIndex):
		buttonX, startY, stepY = self.__GetListButtonLayout()
		return (
			buttonX,
			startY + slotIndex * stepY,
		)

	def __SyncListButtonPositions(self):
		for i, entry in enumerate(self.__title_button_list):
			x, y = self.__GetListRowPosition(i)
			entry["button"].SetPosition(x, y)

	def __ProbeMouseListSlot(self):
		if not wndMgr or not self.__list_board:
			return -1
		try:
			mx, my = wndMgr.GetMousePosition()
			gx, gy = self.__list_board.GetGlobalPosition()
		except:
			return -1
		buttonX, startY, stepY = self.__GetListButtonLayout()
		localX = mx - gx - buttonX
		localY = my - gy - startY
		if stepY <= 0:
			return -1
		if localX < 0 or localX >= self.LIST_BUTTON_WIDTH:
			return -1
		if localY < 0:
			return -1
		slot = int(localY) // stepY
		if slot < 0 or slot >= self.SHOW_BUTTON_MAX:
			return -1
		realIndex = self.__scroll_pos + slot
		if realIndex >= self.__title_cnt:
			return -1
		return slot

	def __DebugLogState(self, tag):
		if not self.DEBUG_LOG:
			return
		buttonX, startY, stepY = self.__GetListButtonLayout()
		self.__DebugLog(
			"%s ui_v%d titles=%d scroll=%d sel=%d layout=(%d,%d,%d)" % (
				tag,
				self.LIST_UI_VERSION,
				self.__title_cnt,
				self.__scroll_pos,
				self.__select_button_index,
				buttonX,
				startY,
				stepY,
			)
		)
		if not self.__title_button_list:
			self.__DebugLog("%s buttons=0 (BUILD FAILED?)" % tag)
			return
		for i in xrange(min(3, len(self.__title_button_list))):
			entry = self.__title_button_list[i]
			btn = entry["button"]
			try:
				x, y = btn.GetLocalPosition()
				w = btn.GetWidth()
				h = btn.GetHeight()
				show = int(btn.IsShow())
				down = 0
				try:
					if btn.IsDown():
						down = 1
				except:
					pass
				self.__DebugLog(
					"row%d local=%d,%d size=%dx%d show=%d down=%d text=%s" % (
						i, x, y, w, h, show, down, entry["name"].GetText()
					)
				)
			except:
				self.__DebugLog("row%d probe failed" % i)

	def __BindListRowButton(self, btn, slotIndex):
		btn.SetEvent(ui.__mem_func__(self.__OnTitleRowSelect), slotIndex)

	def __BuildButtons(self):
		self.__title_button_list = []
		if not self.__list_board:
			return

		for i in xrange(self.SHOW_BUTTON_MAX):
			x, y = self.__GetListRowPosition(i)

			btn = ui.RadioButton()
			btn.SetParent(self.__list_board)
			btn.SetPosition(x, y)
			btn.SetUpVisual(self.BUTTON_DEFAULT)
			btn.SetOverVisual(self.BUTTON_OVER)
			btn.SetDownVisual(self.BUTTON_DOWN)
			self.__BindListRowButton(btn, i)
			btn.Hide()

			nameText = ui.TextLine()
			nameText.SetParent(btn)
			nameText.SetPosition(98, 11)
			nameText.SetHorizontalAlignCenter()
			nameText.SetVerticalAlignCenter()
			nameText.AddFlag("not_pick")
			nameText.Show()

			stateIcon = ui.ImageBox()
			stateIcon.SetParent(btn)
			stateIcon.SetPosition(176, 3)
			stateIcon.AddFlag("not_pick")
			stateIcon.Show()

			stateIconArea = ui.Window()
			stateIconArea.SetParent(btn)
			stateIconArea.SetPosition(176, 3)
			stateIconArea.SetSize(18, 18)
			stateIconArea.SetOverEvent(ui.__mem_func__(self.__OverInTitleRow), i)
			stateIconArea.SetOverOutEvent(ui.__mem_func__(self.__OverOutTitleRow))
			stateIconArea.OnMouseLeftButtonDown = lambda slot=i: self.__OnTitleRowSelect(slot)
			stateIconArea.Show()

			self.__title_button_list.append({
				"button": btn,
				"name": nameText,
				"state": stateIcon,
				"state_area": stateIconArea,
			})
		self.__DebugLog("BuildButtons count=%d parent=list_board" % len(self.__title_button_list))

	def __OnTitleRowSelect(self, slotIndex):
		self.__DebugLog("CallEvent slot=%d" % slotIndex)
		self.__SelectButton(slotIndex)

	def __OverInTitleRow(self, slotIndex):
		if not self.__tooltip:
			return

		realIndex = self.__scroll_pos + slotIndex
		if realIndex < 0 or realIndex >= self.__title_cnt:
			return

		playerInfo = self.__show_title_list[realIndex]
		titleIndex = playerInfo.GetIndex()
		titleData = self.__title_dict.get(titleIndex)
		if not titleData:
			return

		self.__tooltip.ClearToolTip()
		self.__tooltip.toolTipWidth = self.__tooltip.TOOL_TIP_WIDTH

		# Line 1: Title Name in NORMAL_COLOR
		self.__tooltip.AutoAppendTextLine(titleData.GetName(), self.__tooltip.NORMAL_COLOR, True)

		# Line 2: Tooltip message based on obtaining state and permanence
		if not playerInfo.IsObtained():
			# Locked title (Kilit iconu) -> Gereksinim: %s
			condition = titleData.GetConditionText()
			if condition:
				self.__tooltip.AutoAppendTextLine(localeInfo.TITLE_SYSTEM_TOOLTIP_GET_CONDITION % condition, self.__tooltip.NORMAL_COLOR, True)
		else:
			# Unlocked title
			if titleData.IsPermanent():
				# Permanent (Ä°nfiniti iconu) -> KalÄ±cÄ± olarak kullanÄ±labilir.
				self.__tooltip.AutoAppendTextLine(localeInfo.TITLE_SYSTEM_TOOLTIP_USE_INFINITE, self.__tooltip.NORMAL_COLOR, True)
			else:
				# Timed title (Zaman iconu) -> Kalan sÃ¼re: %s
				timeLeft = max(0, playerInfo.GetEndTime() - app.GetGlobalTimeStamp())
				self.__tooltip.AutoAppendTextLine(localeInfo.TITLE_SYSTEM_TOOLTIP_LEFT_TIME % localeInfo.SecondToDHM(timeLeft), self.__tooltip.NORMAL_COLOR, True)

		self.__tooltip.AlignHorizonalCenter()
		self.__tooltip.Show()

	def __OverOutTitleRow(self):
		if self.__tooltip:
			self.__tooltip.Hide()

	def __PathExists(self, fileName):
		try:
			return bool(app.IsExistFile(fileName))
		except:
			return False

	def __NormalizeAssetPath(self, relPath):
		if not relPath:
			return ""
		relPath = relPath.replace("\\", "/")
		if relPath.lower().startswith("d:/ymir work/"):
			return relPath
		return self.YMIR_ROOT + relPath

	def __ResolveImagePath(self, relPath):
		fullPath = self.__NormalizeAssetPath(relPath)
		if not fullPath:
			return ""
		if self.__PathExists(fullPath):
			return fullPath
		if fullPath.lower().endswith(".tga"):
			altPath = fullPath[:-4] + ".dds"
			if self.__PathExists(altPath):
				return altPath
		return ""

	def __GetEffectFileName(self, effectPath):
		if not effectPath:
			return ""
		effectPath = effectPath.replace("\\", "/")
		if "/" in effectPath:
			return effectPath.rsplit("/", 1)[-1]
		return effectPath

	def __GetSpriteResourceIndex(self, resourceIndex, titleIndex):
		return int(resourceIndex)

	def __GetPreviewResourceIndex(self, titleData):
		titleIndex = int(titleData.GetIndex())
		if titleIndex in self.PREVIEW_RESOURCE_BY_TITLE:
			return self.PREVIEW_RESOURCE_BY_TITLE[titleIndex]
		return int(titleData.GetResourceIndex())

	def __SafeCenterX(self, areaWidth, contentWidth):
		try:
			area = int(areaWidth)
		except:
			area = 0
		try:
			content = int(contentWidth)
		except:
			content = 0
		pos = (area - content) / 2
		if pos < 0:
			return 0
		return int(pos)

	def __ReadTextFileLines(self, path):
		if not self.__PathExists(path):
			return []
		try:
			handle = app.OpenTextFile(path)
		except:
			return []
		if handle <= 0:
			return []
		try:
			lineCount = app.GetTextFileLineCount(handle)
		except:
			return []
		lines = []
		for i in xrange(lineCount):
			try:
				line = app.GetTextFileLine(handle, i)
			except:
				continue
			if line:
				lines.append(line)
		return lines

	def __ParseTitleSub(self, resourceIndex):
		if resourceIndex in self.__sub_dict:
			return self.__sub_dict[resourceIndex]

		path = self.TITLE_SUB_PATH % int(resourceIndex)
		data = {}
		if not self.__PathExists(path):
			self.__DebugPreviewTrace("sub.missing", path)
		for rawLine in self.__ReadTextFileLines(path):
			try:
				line = rawLine.strip()
			except:
				continue
			if not line or line.startswith("#"):
				continue
			parts = line.split("\t", 1)
			if len(parts) != 2:
				continue
			data[parts[0].strip()] = parts[1].strip()

		self.__sub_dict[resourceIndex] = data
		return data

	def __SetupRightPanel(self):
		if not self.__right_board:
			return

		self.__preview_bg_box = ui.ImageBox()
		self.__preview_bg_box.SetParent(self.__right_board)
		self.__preview_bg_box.AddFlag("not_pick")
		try:
			self.__preview_bg_box.LoadImage(self.ROOT_PATH + "model_view_background.sub")
		except:
			self.__preview_bg_box = None
		if self.__preview_bg_box:
			self.__preview_bg_box.SetPosition(0, 0)
			try:
				self.__preview_bg_box.SetSize(self.RIGHT_PANEL_WIDTH, 346)
			except:
				pass
			self.__preview_bg_box.Hide()

		if self.__render_target:
			self.__render_target.Show()

		self.__selected_title_name = ui.TextLine()
		self.__selected_title_name.SetParent(self.__right_board)
		self.__selected_title_name.SetHorizontalAlignCenter()
		self.__selected_title_name.SetPosition(self.RIGHT_PANEL_WIDTH / 2, self.SELECTED_NAME_Y)
		self.__selected_title_name.SetText("")
		self.__selected_title_name.Show()

	def __ClearPreviewResource(self):
		if app.RENDER_TARGET:
			titleSystem.SetPreviewResource(0, 0)

	def __ApplyPreviewBackground(self, titleData):
		if app.RENDER_TARGET:
			titleSystem.SetPreviewResource(
				self.__GetPreviewResourceIndex(titleData),
				titleData.GetIndex(),
			)

	def __SetPreviewLayerMode(self, use_native_rt):
		self.__preview_uses_native_rt = bool(use_native_rt)
		if self.__render_target:
			if self.__preview_uses_native_rt:
				self.__render_target.Show()
			else:
				self.__render_target.Hide()
				titleSystem.HideEffect()
				self.__ClearPreviewResource()
		if self.__preview_bg_box:
			if self.__preview_uses_native_rt:
				self.__preview_bg_box.Hide()
			else:
				self.__preview_bg_box.Show()

	def __ClearPreviewWidgets(self):
		titleSystem.HideEffect()
		self.__preview_uses_native_rt = False
		self.__dragon_preview_layers = []
		self.__dragon_preview_start_time = 0.0
		for widget in self.__preview_widgets:
			try:
				widget.Hide()
			except:
				pass
		self.__preview_widgets = []

	def __GetPreviewParent(self):
		return self.__right_board

	def __GetPreviewPlaceholderText(self):
		try:
			return localeInfo.TITLE_SYSTEM_TEXT_TITLE
		except:
			return "Unvan"

	def __MeasurePlaceholderTextWidth(self):
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return 48
		textLine = ui.TextLine()
		textLine.SetParent(previewParent)
		textLine.SetText(self.__GetPreviewPlaceholderText())
		try:
			textW, textH = textLine.GetTextSize()
		except:
			textW = 0
		try:
			textLine.Hide()
		except:
			pass
		if textW <= 0:
			return 48
		return int(textW)

	def __AddPreviewPlaceholderText(self, frameY, frameHeight):
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return
		try:
			frameY = int(frameY)
			frameHeight = int(frameHeight)
		except:
			frameY = self.PREVIEW_BANNER_Y
			frameHeight = 128
		if frameHeight <= 0:
			frameHeight = 128
		textLine = ui.TextLine()
		textLine.SetParent(previewParent)
		textLine.AddFlag("not_pick")
		textLine.SetHorizontalAlignCenter()
		textLine.SetVerticalAlignCenter()
		textLine.SetPosition(self.RIGHT_PANEL_WIDTH / 2, frameY + (frameHeight / 2))
		textLine.SetText(self.__GetPreviewPlaceholderText())
		textLine.SetPackedFontColor(self.PREVIEW_PLACEHOLDER_TEXT_COLOR)
		textLine.Show()
		self.__TrackPreviewWidget(textLine)

	def __ResolvePreviewSpec(self, titleData):
		titleIndex = int(titleData.GetIndex())
		resourceIndex = self.__GetPreviewResourceIndex(titleData)

		if (titleIndex in self.NATIVE_EFFECT_TITLE_IDS or resourceIndex in self.NATIVE_EFFECT_TITLE_IDS) and titleIndex != 1011:
			return "native", None

		subData = self.__ParseTitleSub(resourceIndex)
		if titleIndex == 1011:
			return "nameplate", subData

		subType = subData.get("type", "").upper()

		if subType == "IMAGE":
			return "image", subData.get("image", "")
		if subType == "NAMEPLATE":
			return "nameplate", subData
		if subType == "EFFECT":
			effectName = self.__ResolveEffectFileName(subData, titleData)
			spec = self.EFFECT_PREVIEW_SPECS.get(effectName)
			if spec:
				return spec[0], spec[1]
			return "native", None

		titleType = titleData.GetType()
		if titleType == titleSystem.TYPE_IMAGE:
			return "image", subData.get("image", "")
		if titleType == titleSystem.TYPE_NAMEPLATE:
			return "nameplate", subData
		if titleType == titleSystem.TYPE_EFFECT:
			effectName = self.__ResolveEffectFileName(subData, titleData)
			spec = self.EFFECT_PREVIEW_SPECS.get(effectName)
			if spec:
				return spec[0], spec[1]
			return "native", None

		return "sprite", self.__GetSpriteResourceIndex(resourceIndex, titleIndex)

	def __ApplyPreviewSpec(self, specType, specData, titleData, subData):
		if specType == "native" or specType == "effect":
			return self.__ShowNativeEffectPreview()
		if specType == "dragon":
			return self.__BuildDragonPreview(int(specData))
		if specType == "sprite":
			spriteRes = int(specData)
			spriteSub = subData
			if not spriteSub.get("sprites"):
				spriteSub = {
					"sprites": "ui/game/title/titles/sprites_%04d/" % spriteRes,
					"sprites_count": "36",
					"sprite_size_x": "208",
					"sprite_size_y": "128",
				}
			return self.__BuildSpritePreview(spriteRes, spriteSub, titleData.GetIndex())
		if specType == "image":
			if specData and self.__BuildImagePreview(specData):
				return True
			return self.__TryBuildEffectPreview(subData, titleData)
		if specType == "nameplate":
			if isinstance(specData, dict):
				return self.__BuildNameplatePreview(specData, titleData.GetIndex())
			return self.__BuildNameplateByColorKey(specData)
		return False

	def __BuildImagePreviewWithFallback(self, imagePath, titleData, subData):
		if imagePath and self.__BuildImagePreview(imagePath):
			return True
		return self.__TryBuildEffectPreview(subData, titleData)

	def __ParseSubImageTexturePath(self, subFilePath):
		for rawLine in self.__ReadTextFileLines(subFilePath):
			try:
				line = rawLine.strip()
			except:
				continue
			if not line.lower().startswith("image"):
				continue
			q1 = line.find('"')
			if q1 < 0:
				continue
			q2 = line.find('"', q1 + 1)
			if q2 <= q1:
				continue
			rel = line[q1 + 1:q2].replace("\\", "/")
			if rel.lower().startswith("d:/ymir work/"):
				return rel
			if rel.startswith("game/"):
				return self.__NormalizeAssetPath("ui/" + rel)
			if rel.startswith("ui/"):
				return self.__NormalizeAssetPath(rel)
			return self.__NormalizeAssetPath(rel)
		return ""

	def __SpriteFramesRenderable(self, resourceIndex, subData, titleIndex=0):
		resourceIndex = self.__GetSpriteResourceIndex(resourceIndex, titleIndex)
		spriteDir = subData.get("sprites", "")
		if spriteDir:
			subPath = self.__NormalizeAssetPath(spriteDir.rstrip("/") + "/00.sub")
		else:
			subPath = "d:/ymir work/ui/game/title/titles/sprites_%04d/00.sub" % int(resourceIndex)
		texPath = self.__ParseSubImageTexturePath(subPath)
		if not texPath:
			return False
		return self.__PathExists(texPath)

	def __TrackPreviewWidget(self, widget):
		if widget:
			self.__preview_widgets.append(widget)

	def __GetSpriteDisplaySize(self, subData, resourceIndex):
		try:
			w = int(subData.get("sprite_size_x", 0))
			h = int(subData.get("sprite_size_y", 0))
		except:
			w, h = 0, 0
		if w > 0 and h > 0:
			return w, h
		try:
			info = titleSystem.GetSpriteInfo(int(resourceIndex))
			w = int(info[0])
			h = int(info[1])
			if w > 0 and h > 0:
				return w, h
		except:
			pass
		return 208, 128

	def __ResolveEffectFileName(self, subData, titleData):
		effectPath = subData.get("effect", "")
		if not effectPath and titleData:
			effectPath = self.TITLE_DEFAULT_EFFECT.get(titleData.GetIndex(), "")
		if not effectPath and titleData:
			effectPath = self.TITLE_DEFAULT_EFFECT.get(titleData.GetResourceIndex(), "")
		if not effectPath:
			return ""
		return self.__GetEffectFileName(effectPath)

	def __AppendSpriteFrames(self, aniBox, spriteDir, frameCount):
		basePath = self.__NormalizeAssetPath(spriteDir.rstrip("/"))
		added = 0
		for i in xrange(frameCount):
			path = "%s/%02d.sub" % (basePath, i)
			if not self.__PathExists(path):
				break
			try:
				aniBox.AppendImage(path)
				added += 1
			except:
				break
		return added

	def __BuildSpritePreview(self, resourceIndex, subData, titleIndex=0, yPosition=-1, skipPlaceholder=False):
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return False

		resourceIndex = self.__GetSpriteResourceIndex(resourceIndex, titleIndex)

		frameCount = self.SPRITE_FRAME_COUNT
		try:
			frameCount = int(subData.get("sprites_count", self.SPRITE_FRAME_COUNT))
		except:
			frameCount = self.SPRITE_FRAME_COUNT

		aniBox = ui.AniImageBox()
		aniBox.SetParent(previewParent)
		aniBox.AddFlag("not_pick")
		aniBox.SetDelay(10)

		added = 0
		spriteDir = subData.get("sprites", "")
		if spriteDir:
			added = self.__AppendSpriteFrames(aniBox, spriteDir, frameCount)

		if added <= 0:
			try:
				paths = titleSystem.GetSpriteImages(int(resourceIndex))
				for path in paths:
					if self.__PathExists(path):
						try:
							aniBox.AppendImage(path)
							added += 1
						except:
							break
			except:
				pass

		if added <= 0:
			self.__DebugPreviewTrace(
				"sprite.fail",
				"res=%d title=%d frames=0 dir=%s" % (resourceIndex, titleIndex, spriteDir),
			)
			return False

		self.__DebugPreviewTrace("sprite.ok", "res=%d frames=%d" % (resourceIndex, added))

		try:
			aniBox.ResetFrame()
		except:
			pass

		displayW, displayH = self.__GetSpriteDisplaySize(subData, resourceIndex)
		try:
			w = aniBox.GetWidth()
			h = aniBox.GetHeight()
		except:
			w, h = 0, 0
		if w <= 0:
			w = displayW
		if h <= 0:
			h = displayH
		try:
			aniBox.SetSize(w, h)
		except:
			pass

		width = w if w > 0 else self.NAMEPLATE_TOTAL_WIDTH
		if yPosition == -1:
			yPosition = self.PREVIEW_BANNER_Y
		aniBox.SetPosition(self.__SafeCenterX(self.RIGHT_PANEL_WIDTH, width), yPosition)
		aniBox.Show()
		self.__TrackPreviewWidget(aniBox)
		if not skipPlaceholder:
			self.__AddPreviewPlaceholderText(yPosition, h)
		return True

	def __BuildImagePreview(self, imagePath, withPlaceholder=False):
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return False
		fullPath = self.__ResolveImagePath(imagePath)
		if not fullPath:
			return False

		imageBox = ui.ImageBox()
		imageBox.SetParent(previewParent)
		imageBox.AddFlag("not_pick")
		try:
			imageBox.LoadImage(fullPath)
		except:
			return False

		try:
			w = imageBox.GetWidth()
			h = imageBox.GetHeight()
		except:
			w, h = 0, 0
		if w <= 0:
			w = 128
		if h <= 0:
			h = 128

		x = self.__SafeCenterX(self.RIGHT_PANEL_WIDTH, w)
		y = 120 + self.__SafeCenterX(120, h)
		imageBox.SetPosition(x, y)
		imageBox.Show()
		self.__TrackPreviewWidget(imageBox)
		if withPlaceholder:
			self.__AddPreviewPlaceholderText(y, h)
		return True

	def __BuildDragonPreview(self, titleIndex):
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return False
		layers = self.DRAGON_PREVIEW_LAYERS.get(int(titleIndex))
		if not layers:
			return False

		built = 0
		self.__dragon_preview_layers = []
		self.__dragon_preview_start_time = app.GetTime()
		for layerIndex, layerData in enumerate(layers):
			path, x, y, scale, alpha, delay, fadeTime, mode = layerData
			box, width = self.__MakePreviewImageBox(path, expanded=True)
			if not box:
				continue
			try:
				height = box.GetHeight()
			except:
				height = 0
			if width <= 0:
				width = 128
			if height <= 0:
				height = 128
			try:
				box.SetOrigin(float(width) / 2.0, float(height) / 2.0)
			except:
				pass
			box.SetPosition(int(x), int(y))
			try:
				startScale = float(scale)
				if mode == "dragon":
					startScale *= 0.25
				else:
					startScale *= 0.70
				box.SetScale(startScale, startScale)
			except:
				pass
			self.__SetPreviewLayerAlpha(box, 0.0)
			box.Show()
			self.__TrackPreviewWidget(box)
			self.__dragon_preview_layers.append({
				"box": box,
				"x": int(x),
				"y": int(y),
				"scale": float(scale),
				"alpha": float(alpha),
				"delay": float(delay),
				"fade": float(fadeTime),
				"mode": mode,
				"index": int(layerIndex),
			})
			built += 1

		self.__DebugPreviewTrace("dragon", "title=%d layers=%d" % (int(titleIndex), built))
		self.__UpdateDragonPreview()
		return built > 0

	def __SetPreviewLayerAlpha(self, box, alpha):
		if alpha < 0.0:
			alpha = 0.0
		if alpha > 1.0:
			alpha = 1.0
		try:
			box.SetAlpha(alpha)
			return
		except:
			pass
		try:
			box.SetDiffuseColor(1.0, 1.0, 1.0, alpha)
		except:
			pass

	def __SmoothStep(self, value):
		if value <= 0.0:
			return 0.0
		if value >= 1.0:
			return 1.0
		return value * value * (3.0 - 2.0 * value)

	def __UpdateDragonPreview(self):
		if not self.__dragon_preview_layers:
			return
		try:
			elapsed = app.GetTime() - self.__dragon_preview_start_time
		except:
			return
		cycle = math.fmod(elapsed, 2.35)
		fadeOut = 1.0
		if cycle > 2.08:
			fadeOut = 1.0 - self.__SmoothStep((cycle - 2.08) / 0.27)
		for layer in self.__dragon_preview_layers:
			box = layer.get("box")
			if not box:
				continue
			idx = layer.get("index", 0)
			baseX = layer.get("x", 0)
			baseY = layer.get("y", 0)
			baseScale = layer.get("scale", 1.0)
			baseAlpha = layer.get("alpha", 1.0)
			delay = layer.get("delay", 0.0)
			fadeTime = layer.get("fade", 0.30)
			mode = layer.get("mode", "")

			localTime = cycle - delay
			progress = self.__SmoothStep(localTime / fadeTime)
			if localTime < 0.0:
				progress = 0.0

			phase = cycle * 5.2 + float(idx) * 0.73
			slowPhase = cycle * 2.1 + float(idx) * 1.11
			if mode == "ring":
				scale = baseScale * (0.70 + 0.30 * progress + math.sin(phase) * 0.018)
				alpha = baseAlpha * progress * fadeOut
				x = baseX + int(math.sin(slowPhase) * 1.0)
				y = baseY + int(math.cos(slowPhase) * 1.0)
			else:
				scale = baseScale * (0.25 + 0.75 * progress + math.sin(phase) * 0.025)
				alpha = baseAlpha * progress * fadeOut
				x = baseX + int(math.sin(slowPhase) * 2.0)
				y = baseY + int((1.0 - progress) * 22.0) + int(math.cos(slowPhase) * 1.0)

			try:
				box.SetPosition(x, y)
			except:
				pass
			try:
				box.SetScale(scale, scale)
			except:
				pass
			self.__SetPreviewLayerAlpha(box, alpha)

			if mode == "ring":
				rotation = (cycle * (120.0 if idx == 0 else -70.0)) % 360.0
				try:
					box.SetRotation(rotation)
				except:
					pass
			else:
				try:
					box.SetRotation(math.sin(phase) * 2.5)
				except:
					pass

	def __MakePreviewImageBox(self, path, expanded=False):
		try:
			box = ui.ExpandedImageBox() if expanded else ui.ImageBox()
		except:
			box = ui.ImageBox()
		previewParent = self.__GetPreviewParent()
		if not previewParent:
			return None, 0
		box.SetParent(previewParent)
		box.AddFlag("not_pick")
		fullPath = self.__ResolveImagePath(path)
		if not fullPath:
			return None, 0
		try:
			box.LoadImage(fullPath)
		except:
			return None, 0
		try:
			w = box.GetWidth()
		except:
			w = 0
		return box, (w or 0)

	def __BuildNameplateBannerPreview(self, middlePath):
		if not middlePath:
			return False
		for key, relPath in self.NAMEPLATE_BANNER_DDS.items():
			if key in middlePath:
				return self.__BuildImagePreview(relPath, True)
		return False

	def __BuildNameplateByColorKey(self, colorKey):
		parts = self.NAMEPLATE_TGA.get(colorKey)
		if not parts:
			return False
		leftPath, middlePath, rightPath = parts
		subData = {
			"left_image": leftPath,
			"middle_image": middlePath,
			"right_image": rightPath,
		}
		return self.__BuildNameplatePreview(subData, titleIndex=0, allowSprite=False)

	def __BuildNameplatePreview(self, subData, titleIndex=0, allowSprite=True):
		if not self.__GetPreviewParent():
			return False



		leftPath = subData.get("left_image", "")
		middlePath = subData.get("middle_image", "")
		rightPath = subData.get("right_image", "")
		if not (leftPath and middlePath and rightPath):
			return False

		leftBox, lw = self.__MakePreviewImageBox(leftPath, expanded=False)
		rightBox, rw = self.__MakePreviewImageBox(rightPath, expanded=False)
		middleBox, mw = self.__MakePreviewImageBox(middlePath, expanded=True)
		if not (leftBox and rightBox and middleBox):
			for box in (leftBox, rightBox, middleBox):
				if box:
					try:
						box.Hide()
					except:
						pass
			return self.__BuildNameplateBannerPreview(middlePath)

		if lw <= 0:
			lw = 32
		if rw <= 0:
			rw = 32
		if mw <= 0:
			mw = 1

		textW = self.__MeasurePlaceholderTextWidth()
		middleW = max(mw, textW + self.NAMEPLATE_MIDDLE_PAD_X)
		scaleX = self.NAMEPLATE_STATIC_PREVIEW_SCALE
		scaleY = scaleX * self.NAMEPLATE_STATIC_PREVIEW_SCALE_Y
		scaledLw = max(1, int(lw * scaleX))
		scaledRw = max(1, int(rw * scaleX))
		scaledMiddleW = max(1, int(middleW * scaleX))
		totalW = scaledLw + scaledMiddleW + scaledRw

		baseX = self.__SafeCenterX(self.RIGHT_PANEL_WIDTH, totalW)
		bannerY = self.PREVIEW_BANNER_Y
		leftBox.SetPosition(baseX, bannerY)
		middleBox.SetPosition(baseX + scaledLw, bannerY)
		rightBox.SetPosition(baseX + scaledLw + scaledMiddleW, bannerY)
		try:
			leftBox.SetScale(scaleX, scaleY)
			middleBox.SetScale((float(middleW) / float(mw)) * scaleX, scaleY)
			rightBox.SetScale(scaleX, scaleY)
		except:
			pass

		if allowSprite:
			resourceIndex = 0
			# Check if sub file has sprites
			if subData.get("sprites") or subData.get("sprites_count"):
				try:
					spriteDir = subData.get("sprites", "")
					if "sprites_" in spriteDir:
						resourceIndex = int(spriteDir.split("sprites_")[1].split("/")[0])
				except:
					resourceIndex = 0

			if resourceIndex > 0:
				try:
					frameH = leftBox.GetHeight()
				except:
					frameH = 32
				frameH = max(1, int(frameH * scaleY))
				spriteH = 128
				spriteSub = subData
				if not spriteSub.get("sprites"):
					spriteSub = {
						"sprites": "ui/game/title/titles/sprites_%04d/" % resourceIndex,
						"sprites_count": "36",
						"sprite_size_x": "208",
						"sprite_size_y": "128",
					}
				try:
					displayW, displayH = self.__GetSpriteDisplaySize(spriteSub, resourceIndex)
					if displayH > 0:
						spriteH = displayH
				except:
					pass
				spriteY = bannerY + frameH - spriteH - 5
				self.__BuildSpritePreview(resourceIndex, spriteSub, titleIndex, yPosition=spriteY, skipPlaceholder=True)

		leftBox.Show()
		middleBox.Show()
		rightBox.Show()
		self.__TrackPreviewWidget(leftBox)
		self.__TrackPreviewWidget(middleBox)
		self.__TrackPreviewWidget(rightBox)
		try:
			frameH = leftBox.GetHeight()
		except:
			frameH = 0
		if frameH <= 0:
			try:
				frameH = middleBox.GetHeight()
			except:
				frameH = 32
		if frameH <= 0:
			frameH = 32
		frameH = max(1, int(frameH * scaleY))
		self.__AddPreviewPlaceholderText(bannerY, frameH)
		return True

	def __TryBuildEffectPreview(self, subData, titleData):
		effectName = self.__ResolveEffectFileName(subData, titleData)
		if not effectName:
			return False
		spec = self.EFFECT_PREVIEW_SPECS.get(effectName)
		if not spec:
			return False
		specType = spec[0]
		if specType == "sprite":
			return self.__BuildSpritePreview(spec[1], subData, titleData.GetIndex())
		if specType == "image":
			return self.__BuildImagePreview(spec[1])
		if specType == "nameplate":
			return self.__BuildNameplateByColorKey(spec[1])
		if specType == "effect":
			return self.__ShowNativeEffectPreview()
		return False

	def __ShowNativeEffectPreview(self):
		if not app.RENDER_TARGET:
			self.__DebugPreviewTrace("native.skip", "RENDER_TARGET=0")
			return False
		try:
			ret = titleSystem.ShowEffect()
		except:
			self.__DebugPreviewTrace("native.err", "ShowEffect exception")
			return False
		ok = int(ret) != 0
		if ok:
			self.__preview_uses_native_rt = True
		self.__DebugPreviewTrace("native", "ShowEffect=%d" % int(ret))
		self.__DebugDumpCppPreview("native")
		return ok

	def __BuildPreviewFallback(self, titleData, subData):
		if self.__TryBuildEffectPreview(subData, titleData):
			return True
		if self.__BuildSpritePreview(titleData.GetResourceIndex(), subData, titleData.GetIndex()):
			return True
		if self.__BuildNameplatePreview(subData, titleData.GetIndex()):
			return True
		imagePath = subData.get("image", "")
		if imagePath and self.__BuildImagePreview(imagePath):
			return True
		return False

	def __RefreshSelectedPreview(self):
		self.__ClearPreviewWidgets()
		self.__DebugDumpEnv("refresh")
		if not self.__right_board:
			self.__DebugPreviewTrace("abort", "no right_board")
			return
		if self.__select_button_index < 0 or self.__select_button_index >= self.__title_cnt:
			if self.__selected_title_name:
				self.__selected_title_name.SetText("")
			self.__ClearPreviewResource()
			self.__SetPreviewLayerMode(True)
			return

		playerInfo = self.__show_title_list[self.__select_button_index]
		titleData = self.__title_dict.get(playerInfo.GetIndex())
		if not titleData:
			self.__ClearPreviewResource()
			return

		self.__RefreshSelectedName(titleData)
		self.__ApplyPreviewBackground(titleData)
		previewResourceIndex = self.__GetPreviewResourceIndex(titleData)
		subData = self.__ParseTitleSub(previewResourceIndex)
		specType, specData = self.__ResolvePreviewSpec(titleData)
		branch = "spec." + specType

		self.__DebugPreviewTrace(
			"start",
			"title=%d res=%d pyType=%d spec=%s data=%s subType=%s" % (
				titleData.GetIndex(),
				previewResourceIndex,
				titleData.GetType(),
				specType,
				str(specData),
				subData.get("type", ""),
			),
		)

		built = self.__ApplyPreviewSpec(specType, specData, titleData, subData)
		self.__SetPreviewLayerMode(self.__preview_uses_native_rt)

		previewKey = "%d|%s|%d" % (titleData.GetIndex(), branch, int(built))
		if previewKey != self.__debug_last_preview_key:
			self.__debug_last_preview_key = previewKey
			self.__DebugLog(
				"preview DONE title=%d res=%d branch=%s built=%d widgets=%d layer_rt=%d" % (
					titleData.GetIndex(),
					previewResourceIndex,
					branch,
					int(built),
					len(self.__preview_widgets),
					int(self.__preview_uses_native_rt),
				)
			)
			self.__DebugDumpRtWidget("done")
			self.__DebugDumpPreviewWidgets("done")
			self.__DebugDumpCppPreview("done")

	def __RefreshSelectedName(self, titleData):
		if not self.__selected_title_name:
			return
		self.__selected_title_name.SetText(titleData.GetName())
		try:
			self.__selected_title_name.SetPackedFontColor(0xFFD1D1D1)
		except:
			pass
		self.__selected_name_scroll = 0
		self.__diff = 0

	def __UpdateSelectedNameScroll(self):
		if not self.__selected_title_name:
			return
		try:
			textWidth, textHeight = self.__selected_title_name.GetTextSize()
		except:
			return
		if textWidth <= self.MAX_TEXT_WIDTH:
			if self.__selected_name_scroll != 0:
				self.__selected_title_name.SetPosition(
					self.RIGHT_PANEL_WIDTH / 2,
					self.SELECTED_NAME_Y,
				)
				self.__selected_name_scroll = 0
				self.__diff = 0
			return

		self.__diff += 1
		if (self.__diff % 2) != 0:
			return

		self.__selected_name_scroll += self.MOVING_DIRECTION
		limit = textWidth - self.MAX_TEXT_WIDTH
		if self.__selected_name_scroll < -limit:
			self.__selected_name_scroll = 0
		if self.__selected_name_scroll > 0:
			self.__selected_name_scroll = -limit

		self.__selected_title_name.SetPosition(
			self.RIGHT_PANEL_WIDTH / 2 + self.__selected_name_scroll,
			self.SELECTED_NAME_Y,
		)

	def __LoadData(self):
		self.__title_dict = {}
		allData = titleSystem.GetAllTitleData()
		for row in allData:
			info = TitleInfo(row)
			self.__title_dict[info.GetIndex()] = info

		self.__show_title_list = []
		showList = titleSystem.GetShowList()
		for row in showList:
			self.__show_title_list.append(PlayerTitleInfo(row))
		self.__title_cnt = len(self.__show_title_list)

	def Refresh(self):
		if not self.isLoaded:
			self.__LoadWindow()
		self.__LoadData()
		if self.DEBUG_LOG:
			obtainCnt = 0
			for playerInfo in self.__show_title_list:
				if playerInfo.IsObtained():
					obtainCnt += 1
			self.__DebugLog("player titles obtained=%d/%d" % (obtainCnt, self.__title_cnt))
		self.__SyncListButtonPositions()
		self.__RefreshScrollBar()
		self.__RefreshButtons()
		self.__RefreshEquippedName()
		self.__RefreshSelectedPreview()

	def __RefreshScrollBar(self):
		if not self.__scroll_bar:
			return
		if self.__title_cnt <= self.SHOW_BUTTON_MAX:
			self.__scroll_bar.Hide()
			self.__scroll_pos = 0
			return
		self.__scroll_bar.Show()
		self.__scroll_bar.SetMiddleBarSize(float(self.SHOW_BUTTON_MAX) / float(self.__title_cnt))

	def __OnScroll(self):
		maxPos = max(0, self.__title_cnt - self.SHOW_BUTTON_MAX)
		self.__scroll_pos = int(self.__scroll_bar.GetPos() * maxPos + 0.5)
		self.__DebugLog("scroll pos=%d" % self.__scroll_pos)
		self.__RefreshButtons()

	def __RefreshButtons(self):
		for i in xrange(self.SHOW_BUTTON_MAX):
			entry = self.__title_button_list[i]
			realIndex = self.__scroll_pos + i
			if realIndex >= self.__title_cnt:
				entry["button"].Hide()
				continue

			playerInfo = self.__show_title_list[realIndex]
			titleIndex = playerInfo.GetIndex()
			titleData = self.__title_dict.get(titleIndex)
			if not titleData:
				entry["button"].Hide()
				continue

			btn = entry["button"]
			btn.Show()
			btn.Enable()
			entry["name"].SetText(titleData.GetName())

			if not playerInfo.IsObtained():
				upVisual = self.BUTTON_DEFAULT_DISABLE
				overVisual = self.BUTTON_OVER
				downVisual = self.BUTTON_DOWN
				entry["state"].LoadImage(self.STATE_LOCKED)
			elif playerInfo.IsEquipped():
				upVisual = self.BUTTON_DEFAULT_EQUIPPED
				overVisual = self.BUTTON_OVER
				downVisual = self.BUTTON_DOWN
				if titleData.IsPermanent():
					entry["state"].LoadImage(self.STATE_PERMANENT)
				else:
					entry["state"].LoadImage(self.STATE_HOURGLASS)
			else:
				upVisual = self.BUTTON_DEFAULT
				overVisual = self.BUTTON_OVER
				downVisual = self.BUTTON_DOWN
				if titleData.IsPermanent():
					entry["state"].LoadImage(self.STATE_PERMANENT)
				else:
					entry["state"].LoadImage(self.STATE_HOURGLASS)

			btn.SetUpVisual(upVisual)
			btn.SetOverVisual(overVisual)
			btn.SetDownVisual(downVisual)

			# CRadioButton keeps pressed state (see PythonWindow.cpp CRadioButton).
			if realIndex == self.__select_button_index:
				btn.Down()
			else:
				btn.SetUp()

	def __RefreshEquippedName(self):
		if not self.__equipped_name:
			return
		equipped = titleSystem.GetEquippedTitle()
		if equipped <= 0:
			self.__equipped_name.SetText("")
			if self.__back_button:
				self.__back_button.Hide()
			return
		titleData = self.__title_dict.get(equipped)
		if titleData:
			self.__equipped_name.SetText(titleData.GetName())
			if self.__back_button:
				self.__back_button.Show()
		else:
			self.__equipped_name.SetText("")
			if self.__back_button:
				self.__back_button.Hide()

	def __SelectButton(self, idx):
		realIndex = self.__scroll_pos + idx
		if realIndex < 0 or realIndex >= self.__title_cnt:
			self.__DebugLog("Select REJECT slot=%d real=%d cnt=%d" % (idx, realIndex, self.__title_cnt))
			return
		self.__select_button_index = realIndex
		titleName = ""
		try:
			titleIndex = self.__show_title_list[realIndex].GetIndex()
			if titleIndex in self.__title_dict:
				titleName = self.__title_dict[titleIndex].GetName()
		except:
			pass
		playerInfo = self.__show_title_list[realIndex]
		self.__DebugLog(
			"Select OK slot=%d real=%d name=%s obtain=%d equip=%d" % (
				idx,
				realIndex,
				titleName,
				int(playerInfo.IsObtained()),
				int(playerInfo.IsEquipped()),
			)
		)
		self.__RefreshButtons()
		self.__RefreshSelectedPreview()

	def __SelectByTitleIndex(self, titleIndex):
		for i, playerInfo in enumerate(self.__show_title_list):
			if playerInfo.GetIndex() == titleIndex:
				self.__select_button_index = i
				break
		self.__RefreshButtons()
		self.__RefreshSelectedPreview()

	def __RequestEquip(self):
		self.__DebugLog("Apply click sel=%d" % self.__select_button_index)
		if self.__select_button_index < 0 or self.__select_button_index >= self.__title_cnt:
			self.__DebugLog("Apply REJECT no selection")
			return
		playerInfo = self.__show_title_list[self.__select_button_index]
		if not playerInfo.IsObtained():
			self.ChatInfo(titleSystem.WRONG_APPROACH, playerInfo.GetIndex())
			return
		titleSystem.RequestEquip(playerInfo.GetIndex())

	def __RequestUnEquip(self):
		self.__DebugLog("Clear click")
		titleSystem.RequestEquip(0)

	def __ClickBackButton(self):
		equipped = titleSystem.GetEquippedTitle()
		if equipped <= 0:
			return
		self.__SelectByTitleIndex(equipped)
		if self.__select_button_index >= 0:
			maxPos = max(0, self.__title_cnt - self.SHOW_BUTTON_MAX)
			if maxPos > 0:
				targetScrollPos = max(0, min(maxPos, self.__select_button_index - self.SHOW_BUTTON_MAX / 2))
				self.__scroll_pos = targetScrollPos
				self.__scroll_bar.SetPos(float(targetScrollPos) / float(maxPos))
				self.__RefreshButtons()

	def __OverInBackButton(self):
		if not self.__tooltip:
			return
		equipped = titleSystem.GetEquippedTitle()
		if equipped <= 0:
			return
		self.__tooltip.ClearToolTip()
		self.__tooltip.AppendTextLine(localeInfo.TITLE_SYSTEM_TOOLTIP_BACK_BUTTON)
		self.__tooltip.Show()

	def __OverOutBackButton(self):
		if self.__tooltip:
			self.__tooltip.Hide()

	def OnTitleNotify(self, messageCode, titleIndex):
		self.ChatInfo(messageCode, titleIndex)
		self.Refresh()

	def OnTitleDataEnd(self):
		self.__DebugLog("OnTitleDataEnd")
		self.Refresh()

	def ChatInfo(self, messageCode, titleIndex):
		titleName = ""
		if titleIndex in self.__title_dict:
			titleName = self.__title_dict[titleIndex].GetName()

		msgMap = {
			titleSystem.ALREAY_GET: localeInfo.TITLE_SYSTEM_MESSAGE_ALREADY_GET,
			titleSystem.WRONG_APPROACH: localeInfo.TITLE_SYSTEM_MESSAGE_WRONG_APPROACH,
			titleSystem.WRONG_ITEM: localeInfo.TITLE_SYSTEM_MESSAGE_WRONG_ITEM,
			titleSystem.END_TITLE: localeInfo.TITLE_SYSTEM_MESSAGE_TIME_END,
			titleSystem.GET_TITLE: localeInfo.TITLE_SYSTEM_MESSAGE_GET_TITLE,
			titleSystem.EQUIP_TITLE: localeInfo.TITLE_SYSTEM_MESSAGE_EQUIP_TITLE,
			titleSystem.UNEQUIP_TITLE: localeInfo.TITLE_SYSTEM_MESSAGE_UNEQUIP_TITLE,
			titleSystem.ONE_HOUR_LEFT: localeInfo.TITLE_SYSTEM_MESSAGE_ONE_HOUR_LEFT,
			titleSystem.TRY_LATER: localeInfo.TITLE_SYSTEM_MESSAGE_TRY_LATER,
			titleSystem.CHECK_UI: localeInfo.TITLE_SYSTEM_MESSAGE_CHECK_UI,
		}

		fmt = msgMap.get(messageCode, "")
		if not fmt:
			return
		try:
			if "%s" in fmt:
				chat.AppendChat(chat.CHAT_TYPE_INFO, fmt % titleName)
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, fmt)
		except:
			chat.AppendChat(chat.CHAT_TYPE_INFO, fmt)
