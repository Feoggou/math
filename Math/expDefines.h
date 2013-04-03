//general math
#define M_E         2.71828182845904523536
#define M_PI        3.14159265358979323846
//error checking
#define MAX_FLOAT	999999999999

//SearchGroup
#define SHGROUP_OPERATOR	1
#define SHGROUP_PARANTHESIS	2
#define SHGROUP_COMMA		3
#define SHGROUP_EQUAL		4
#define SHGROUP_NUMBER		5
#define SHGROUP_SPACE		6
#define SHGROUP_NOGROUP		0

//errors:
#define ERR_EQUALNOTLAST	10
#define ERR_MULTIPLEEQUAL	11

//functions & constants:
#define FUNC_E		1
#define FUNC_PI		2
#define FUNC_RAD	10
#define FUNC_LOG	11
#define FUNC_LN		12
#define FUNC_LG		13
#define FUNC_CMMDC	20
#define FUNC_CMMMC	21
#define FUNC_MIN	30
#define FUNC_MAX	31
#define FUNC_P		40
#define FUNC_A		41
#define FUNC_C		42
#define FUNC_MOD	50
#define FUNC_NOFUNC	0

//character types
#define CHARTYPE_ADDITION			1
#define CHARTYPE_MULTIPLICATION		2
#define CHARTYPE_FACTORIAL			3
#define CHARTYPE_OPENEDPARANTHESIS	4
#define CHARTYPE_CLOSEDPARANTHESIS	5
#define CHARTYPE_COMMA				7
#define CHARTYPE_EQUAL				8
#define CHARTYPE_SPACE				10
#define CHARTYPE_NUMBER				9
#define CHARTYPE_OTHER				0

//names
#define NAME_ERROR			0
#define NAME_NUMBER			1
#define NAME_FUNCTION		3
#define NAME_OPERATOR		4
#define NAME_PARANTHESIS	2
#define NAME_EQUAL			5