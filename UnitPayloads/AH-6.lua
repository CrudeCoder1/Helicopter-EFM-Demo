local unitPayloads = {
	["name"] = "AH-6J",
	["payloads"] = {
		[1] = {
			["name"] = "M134 minigun *2, Hydra 70 *14",
			["pylons"] = {
				[1] = {
					["CLSID"] = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}",
					["num"] = 1,
				},
				[2] = {
					["CLSID"] = "{M134 Minigun}",
					["num"] = 2,
				},
				[5] = {
					["CLSID"] = "{M134 Minigun}",
					["num"] = 5,
				},
				[6] = {
					["CLSID"] = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}",
					["num"] = 6,
				},
			},
			["tasks"] = {
				[1] = 31,
				[2] = 32,
			},
		},

		[2] = {
			["name"] = "M134 minigun, Hydra 70 *7, GAU-19 .50 Cal",
			["pylons"] = {
				[1] = {
					["CLSID"] = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}",
					["num"] = 1,
				},
				[2] = {
					["CLSID"] = "{M134 Minigun}",
					["num"] = 2,
				},
				[6] = {
					["CLSID"] = "{AH6_GAU-19}",
					["num"] = 6,
				},
			},
			["tasks"] = {
				[1] = 31,
				[2] = 32,
			},
		},

	},
	["tasks"] = {
	},
	["unitType"] = "AH-6J",
}
return unitPayloads
