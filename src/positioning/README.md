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
  |hotfix|핫픽스|-|긴급수정으로 바로 배포 브랜치로 병함|
  - [main(master) branch]
    - 최종적으로 배포되는 브랜치
    - 
  - [develop branch]
    - 개발을 지행하는 브랜치
    - 하나의 Feature 브랜치가 Merge될 대마나 develop 브랜치에 해당 기능이 추가
    - develop 브랜치느 ㄴ배포할 기능이 모두 갖추어지면 master 브랜치로 Merge함(Release 가 별도로 존재하지만 생략하고 master merge)
    - 
  - [feature/fix branch]
    - 기능 구현을 담당
    - 브랜치명 컨벤션
      - {브랜치유형}/{이슈번호}-{내용간단요약}
      - (예) 브랜치 유형은 bugfix, 이슈번호
  
  - [hotfix branch]
    - 배포된 소스에서 버그가 발생하여 긴급 수정되어야 하는 경우 생성
    - 수정이 완료되면 master, develop 브랜치에 모두 반영