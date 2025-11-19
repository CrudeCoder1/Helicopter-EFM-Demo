local unitPayloads = {
	["name"] = "AH-6",
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
				[4] = {
					["CLSID"] = "{M134 Minigun}",
					["num"] = 4,
				},
				[5] = {
					["CLSID"] = "{A021F29D-18AB-4d3e-985C-FC9C60E35E9E}",
					["num"] = 5,
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
				[5] = {
					["CLSID"] = "{AH6_GAU-19}",
					["num"] = 5,
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
	["unitType"] = "AH-6",
}
return unitPayloads
