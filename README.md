
## Overview

Origin : https://github.com/GMahmoud/virtual_costmap_layer.git
map 상에서 특정한 구역에 layer 를 설정하고, 그 위에 로봇이 있는지 등을 확인 할 수 있는 pkg 이다. 

* sample.launch : layer를 구현, 확인해보자. //robot 이 layer 안에 있는지 등은 확인 할 수 없음.

## Setup


0. Install dependencies
이 repository 를 git clone 한 이후의 과정.
pkg 에 수록된 version 파일을 catkin_ws/src 로 빼준다.
그리고, catkin_ws/src 에 아래의 custom_msgs 를 git clone 해온다.
```bash
git clone https://github.com/GMahmoud/custom_msgs_demo.git
```

1. Run
  1-1. Run for sample.launch
    단순히 form(생성한 레이어)를 확인 하고 싶을때, 다음의 launch 를 켜라.
  ```bash
  roslaunch virtual_costmap_layer sample.launch
  ```

  1-2. Run for working(Kobuki gazebo)
  https://github.com/3watt/kobuki-metapackage 에서 kobuki-metapackage 를 설치하시오.
  아래의 런치를 실행하고, 노드를 실행시키면 가제보 상에서 layer 안으로 goal 을 찍으면, Parameter(inflation radius) 가 변경되는 것을 확인 할 수 있다.

  ```bash
  roslaunch kobuki_metapackage gazebo_for_layer.launch
  rosrun virtual_costmap_layer param_changed.py
  ```
  +) form 을 get param 하는 위치 사용환경(move_base, local, global 등등)에 따라 변경해주어야 한다. (cmake 하는 것도 잊지 말자!)
  https://github.com/3watt/v_slowdown_layer/blob/abee2528b4dc5be45f4e8b4fe61ba428d4353131/src/main.cpp#L32





## ETC

대략적 추가사항:

1. cost value 변경 (원본에서는 costvalue를 LETHAL_OBSTACLES (255) 로 주어서, 장애물로 인식하였는데, 적당한 값(3) 으로 변경하여, 네비게이션을 했을 때, 생성해준 레이어를 피해 가지 않는다.
  - cost value 변경 

```bash
   setPolygonCost(master_grid, _form_polygons[i], 3 , min_i, min_j, max_i, max_j, true);

```


2. yaml 파일에 있는 form parameter 에 적혀진 좌표들로 만든 polygon에 cost, 바운드 등을 주어, map의 원하는 위치에 layer을 그릴수 있다.
  원본 파일에서는 yaml 에 있는 parameter(form) 좌표에 cost를 부여하거나, bound 를 넣는 부분이 없었다.


3. 로봇이 생성한 레이어 위에 있는지를 보는 robot_in_zone 함수를 새로 생성한 node에서 실행을 시켜 실시간으로 로봇의 pose를 받아와 bool 형의 topic을 publish 해준다.

```bash
// 위치 : src/main.cpp
add_executable(virtual_layer_node src/main.cpp)
target_link_libraries(virtual_layer_node ${catkin_LIBRARIES} virtual_layer)
```




+) 번외..

obstacle topic 으로 map에 그려보고 싶으신 분들을 위해!!

rostopic pub /virtual_costamp_layer/obsctacles custom_msgs/Obstacles "list:
- form:
  - x: 0.0
    y: 0.0 
  - x: 1.0
    y: 1.0
  - x: 2.0
    y: 1.0"
   
   다음의 문장을 넣으시면 볼 수 있습니다. 위는 삼각형입니다. 각각의 x,y 가 꼭짓점이라고 생각하시면 됩니다!
   그리고 원을 원하시면,
   
   rostopic pub /virtual_costamp_layer/obsctacles custom_msgs/Obstacles "list:
- form:
  - x: 0.0
    y: 0.0 
    z: 1.0
    
    
    으로 topic 을 주시면 (0,0) 에서 반지름이 1인 원이 만들어집니다! 여기서 z 는 반지름을 의미합니다. 
    자세한 내용은 업무 보드의 "제임스의 똑똑한 움직임" 의 "virtual layer" 부분을 참고하시면 됩니다!
    위 topic echo 부분이 "-" 이런 기호들이 있어 그대로 복사하시면 안됩니다! 수정 마크를 클릭하셔서 원본을 복사하셔서 echo하시길 바랍니다!! :)

