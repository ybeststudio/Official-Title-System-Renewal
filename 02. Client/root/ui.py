# In `LoadChildren`, extend the elif-statement with:
			elif Type == "render_target":
				if app.ENABLE_MONSTER_CARD or app.ENABLE_MYSHOP_DECO or app.ENABLE_MINI_GAME_YUTNORI or app.ENABLE_TITLE_SYSTEM:
					parent.Children[Index] = RenderTarget()
					parent.Children[Index].SetParent(parent)
					self.LoadElementRenderTarget(parent.Children[Index], ElementValue, parent)


# Add anywhere in the ReadingWnd class:
if app.ENABLE_MONSTER_CARD or app.ENABLE_MYSHOP_DECO or app.ENABLE_MINI_GAME_YUTNORI or app.ENABLE_TITLE_SYSTEM:
	class RenderTarget(Window):

		def __init__(self, layer = "UI"):
			Window.__init__(self, layer)

			self.number = -1

		def __del__(self):
			Window.__del__(self)

		def RegisterWindow(self, layer):
			self.hWnd = wndMgr.RegisterRenderTarget(self, layer)

		def SetRenderTarget(self, number):
			self.number = number
			wndMgr.SetRenderTarget(self.hWnd, self.number)
