dofile(LockOn_Options.script_path.."RWR/Indicator/definitions.lua")


num_points = 32
step = math.rad(360.0/num_points)
TFOV  = .8 * RWR_SCALE
verts = {}
for i = 1, num_points do
	verts[i] = {TFOV * math.cos(i * step), TFOV * math.sin(i * step)}
end


inds = {}
j = 0
for i = 0, 29 do
	j = j + 1
	inds[j] = 0
	j = j + 1
	inds[j] = i + 1
	j = j + 1
	inds[j] = i + 2
end



RWR_base               	= CreateElement "ceSimple"
RWR_base.name			= "RWR_base"
RWR_base.init_pos		= center
Add(RWR_base)

---------------------------------------------------
local 	total_field_of_view 				= CreateElement "ceMeshPoly"
		total_field_of_view.name 			= "total_field_of_view"
		total_field_of_view.vertices		= verts
		total_field_of_view.indices		 	= inds
		total_field_of_view.material		= MakeMaterial(nil,{255,1,1,255})
		total_field_of_view.h_clip_relation = h_clip_relations.REWRITE_LEVEL
		total_field_of_view.level			= RWR_DEFAULT_NOCLIP_LEVEL
		total_field_of_view.isdraw			= true
		total_field_of_view.collimated 		= false
		total_field_of_view.isvisible		= false 
		total_field_of_view.parent_element	= "RWR_base"
	Add(total_field_of_view)
---------------------------------

local 	black_background     			= CreateElement "ceTexPoly"
		black_background.name			="black_background"
		black_background.material      	= MakeMaterial(nil,{30, 30, 30, 255})	
		black_background.vertices		= verts
		black_background.indices    	= inds
		black_background.parent_element = "RWR_base"
		black_background.h_clip_relation= h_clip_relations.INCREASE_IF_LEVEL
		black_background.level	  		= RWR_DEFAULT_NOCLIP_LEVEL
		black_background.isvisible		= false
	Add(black_background)