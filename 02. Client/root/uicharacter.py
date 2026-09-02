# Add to the imports:
if app.ENABLE_TITLE_SYSTEM:
	import uicharactertitle


# In `__Initialize`, extend the if-statement with:
		if app.ENABLE_TITLE_SYSTEM:
			self.wndTitleSystem = None
			self.titleSystemButton = None


# In `__BindObject`, extend the if-statement with:
		if app.ENABLE_TITLE_SYSTEM:
			try:
				self.titleSystemButton = self.GetChild("Face_Button")
				self.titleSystemButton.SetEvent(ui.__mem_func__(self.__ToggleTitleSystemWindow))
			except:
				self.titleSystemButton = None


# In `Destroy`, extend the if-statement with:
		if app.ENABLE_TITLE_SYSTEM:
			if self.wndTitleSystem:
				self.wndTitleSystem.Destroy()
				self.wndTitleSystem = None

# In `Close`, extend the if-statement with:
		if app.ENABLE_TITLE_SYSTEM:
			if self.wndTitleSystem and self.wndTitleSystem.IsShow():
				self.wndTitleSystem.Hide()


# Add anywhere in the CharacterWindow class:
	if app.ENABLE_TITLE_SYSTEM:
		def __ToggleTitleSystemWindow(self):
			if not (hasattr(app, "ENABLE_TITLE_SYSTEM") and app.ENABLE_TITLE_SYSTEM):
				return

			if self.wndTitleSystem and self.wndTitleSystem.IsShow():
				self.wndTitleSystem.Close()
				return

			if self.wndTitleSystem:
				self.wndTitleSystem.Destroy()
				self.wndTitleSystem = None

			self.wndTitleSystem = uicharactertitle.CharacterTitleWindow()
			if self.interface:
				self.wndTitleSystem.BindInterface(self.interface)
			self.wndTitleSystem.Open()


# In `OnUpdate`, extend the if-statement with:
		if app.ENABLE_TITLE_SYSTEM:
			if self.wndTitleSystem and self.wndTitleSystem.IsShow():
				try:
					self.wndTitleSystem.OnUpdate()
				except:
					pass
