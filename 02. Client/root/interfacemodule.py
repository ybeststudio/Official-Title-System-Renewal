# Add anywhere in the Interface class:
	if app.ENABLE_TITLE_SYSTEM:
		def OpenCharacterTitleWindow(self):
			if self.wndCharacter:
				self.wndCharacter.__ToggleTitleSystemWindow()

		def CloseCharacterTitleWindow(self):
			if self.wndCharacter and self.wndCharacter.wndTitleSystem:
				self.wndCharacter.wndTitleSystem.Close()
