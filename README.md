# [KETI]
 - update -K-AutoDrivingLocalization algorithm v1.0
#
## 1. 브랜치 운영 규칙
  |브랜치 유형|의미|담당자|비고|
  |:----------:|:----:|:-----:|:----|
  |main|배포용|윤상훈 책임|최종 배포용 |
  |develop|개발용|장성현 책임 or 안병만 책임|검증된 기능이나 동작을 추가|
  |feature|기능추가| - |새로운 기능이나 동작을 추가|
  |bugfix|버그수정|-|기존 기능의 버그를 수정|
  |hotfix|핫픽스|-|긴급수정으로 바로 배포 브랜치로 병합|
  - [main(master) branch]
    - 최종적으로 배포되는 브랜치
    - 
  - [develop branch]
    - 개발을 지행하는 브랜치
    - 하나의 Feature 브랜치가 Merge될 대마나 develop 브랜치에 해당 기능이 추가
    - develop 브랜치는 배포할 기능이 모두 갖추어지면 master 브랜치로 Merge함(Release 가 별도로 존재하지만 생략하고 master merge)
    - 
  - [feature/fix branch]
    - 기능 구현을 담당
    - 브랜치명 컨벤션
      - {브랜치유형}/{이슈}{커밋유형번호}--{내용간단요약}
      - (예) 브랜치 유형은 bugfix, 이슈는 PU, 커밋유형번호는 2, 수정내용은 메모리릭 버그인 경우
        - bugfix/PU-2--memory-leak
  
  - [hotfix branch]
    - 배포된 소스에서 버그가 발생하여 긴급 수정되어야 하는 경우 생성
    - 수정이 완료되면 master, develop 브랜치에 모두 반영
#
## 2. 이슈 규칙

|이슈 유형|의미|비고|
|:---:|:---:|:---|
|P|Python|Python 기반의 코드|
|M|Matlab|Matlab 기반의 코드|
|I|Feat|IMU 관련|
|U|Feat|IMU 관련|
|R|ROS|ROS 관련|

#
## 3. 커밋 메세지 규칙
|TYPE|번호|내용|
|:---:|:--:|:---|
|fix|1|버그 수정|
|feature|2|새로운 기능추가 및 변경|
|refactor|3| 리팩토링|
|style|4|코드 스타일 수정|
|docs|5|문서수정|
|test|6| 테스트, 유닛테스트 관련 수정|
|build|7| 빌드 관련 수정|

#
## 4. git 실습
  -  root 폴더에 .gitmessage.txt. 생성

    $ touch .gitmessage.txt  or
    $ vim .gitmessage.txt
    $ git config --global commit.template <.gitmessage.txt 경로>
    $ git commit #입력시 위에서 지정된 쳄플릿 파일이 뜨게 되며 커밋 제목, 본문, 꼬릿말 추가




