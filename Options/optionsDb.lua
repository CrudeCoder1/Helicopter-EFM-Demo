local DbOption  = require('Options.DbOption')
local i18n	    = require('i18n')

local _ = i18n.ptranslate

return {

	aimingMarkRemove	= DbOption.new():setValue(false):checkbox(),
	aimingMarkOption	= DbOption.new():setValue(3):combo({DbOption.Item(_('NONE')):Value(0),
															DbOption.Item(_('DOT')):Value(1),
															DbOption.Item(_('X')):Value(2),
															DbOption.Item(_('+')):Value(3),
															DbOption.Item(_('CIRCLE')):Value(4),
															}),
}
