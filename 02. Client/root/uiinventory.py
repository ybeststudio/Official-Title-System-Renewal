# In the `__UseItem` function, find this line:
		item.SelectItem(ItemVNum)

# Add after it:
		if ItemVNum in (57000, 57001, 57002):
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.TITLE_SYSTEM_USE_SCROLL_QUESTION)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnAccept))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.__UseItemQuestionDialog_OnCancel))
			self.questionDialog.Open()
			self.questionDialog.slotIndex = slotIndex
			self.questionDialog.slotWindow = slotWindow
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			return

# Note: the vnum list above is the certificate item list of this package.
# If you use different vnums, change them here, in uitooltip.py and in
# CTitleSystemManager::UseTitleItem (title_system.cpp) together.
