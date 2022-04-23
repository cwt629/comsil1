echo 'Working directory' #sogang/under/cse20161255/shell
read dirname # directory 이름을 입력 후 저장

if [ -n "$dirname" ] # directory를 입력한 경우
then
if [ -e $dirname ] # 해당 directory가 존재하는 경우
then
cd $dirname
for dir in * # 현재 디렉토리의 모든 파일에 대한 for문
do
if [ -f $dir ] # 파일만 골라서 진행하며, 디렉토리는 대소문자가 바뀌지 않게 함
then
newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` #변수 newname은 dir의 대소문자를 tr을 이용해서 바꾼것
mv $dir $newname #$dir을 newname으로 바꾼다.
fi
done
else echo 'Error: Check if such directory exists.' #해당 directory가 존재하지 않는 경우
fi
else #아무것도 입력하지 않고 엔터를 입력한 경우
for dir in *
do
newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`
mv $dir $newname
done
fi
