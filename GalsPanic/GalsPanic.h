#pragma once

#include "resource.h"
#include "windowsx.h"

#include "SceneManager.h"
#include "SceneState.h"
#include "SceneLogin.h"
#include "SceneMain.h"

#include "InputManager.h"

// 머신
#include "Machine.h"
#include "Enemy.h"

/*********************************************************************************
 Collision table
---------------+-------------+------------+--------------------------------------
               | Enemy       | User       | Screen     | Map        |
---------------+-------------+------------+--------------------------------------
 Enemy         | X           | O          | O          | O          | 
 User          | O           | X          | O          | O          | 
 Screen        | O           | O          | X          | X          | 
 Map           | O           | O          | X          | X          | 
 
---------------+-------------+------------+--------------------------------------
*********************************************************************************/

// 행렬 위쪽만 정의. 어차피 아래쪽과 중복임

#define COLLISION_ENEMY_ENEMY false
#define COLLISION_ENEMY_USER true
#define COLLISION_ENEMY_SCREEN true
#define COLLISION_ENEMY_MAP true

#define COLLISION_User_USER false
#define COLLISION_User_SCREEN true
#define COLLISION_User_MAP true

#define COLLISION_SCREEN_SCREEN false
#define COLLISION_SCREEN_MAP false

#define COLLISION_MAP_MAP false

// 상태
#include "State.h"

