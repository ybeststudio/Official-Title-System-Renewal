# In the `AddItemData_Offline` function, find this block:
		### Use ###
		elif item.ITEM_TYPE_USE == itemType:
			self.__AppendLimitInformation()

# Add after it:
			if item.USE_TITLE == itemSubType:
				# Resolve titleIndex dynamically
				titleIndex = 0
				if metinSlot and len(metinSlot) > 1 and metinSlot[1] > 0:
					titleIndex = int(metinSlot[1])
				else:
					titleIndex = int(item.GetValue(0))

				if titleIndex == 0:
					# Fallback based on itemVnum if not set in socket/value
					if itemVnum == 57000:
						titleIndex = 1001
					elif itemVnum == 57001:
						titleIndex = 1000
					elif itemVnum == 57002:
						titleIndex = 1005

				import uicharactertitle
				details = uicharactertitle.GetTitleDetails(titleIndex)
				desc = details["description"]
				isPermanent = details["is_permanent"]

				if desc:
					if "." in desc:
						desc = desc.split(".")[0].strip() + "."
					self.AutoAppendTextLine(desc, self.NORMAL_COLOR)

				# Find duration limit
				limitValue = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValueVal) = item.GetLimit(i)
					if item.LIMIT_REAL_TIME == limitType:
						limitValue = limitValueVal
						break

				# Replicate C++ GetItemDurationSec logic:
				if limitValue <= 0:
					# check metinSlot[0] (expiration timestamp)
					if metinSlot and len(metinSlot) > 0 and metinSlot[0] > 0:
						now = app.GetGlobalTimeStamp()
						if metinSlot[0] > now:
							limitValue = metinSlot[0] - now
						else:
							limitValue = metinSlot[0]

					# check value 1
					if limitValue <= 0:
						limitValue = item.GetValue(1)

				# Print duration information
				self.AppendSpace(5)
				self.AutoAppendTextLine(localeInfo.TITLE_SYSTEM_TOOLTIP_TITLE_TIME_LEFT, self.NORMAL_COLOR)
				if limitValue <= 0:
					limitValue = 31536000 # 365 days default
				self.AutoAppendTextLine(localeInfo.SecondToDay(limitValue), self.NORMAL_COLOR)

				# Align all text lines to the new center
				self.AlignHorizonalCenter()

# Note: this block must come before the existing `elif item.USE_POTION == itemSubType`
# branch, otherwise the certificate tooltip falls through to the potion branch.
