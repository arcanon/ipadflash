url=http://www.youtube.com/watch?v=kpFp5hhCZ4w
vars=$(cat url | grep -F watch-vfl175721.swf | cut -d \? -f 2 | cut -d \" -f1)
echo "$vars"

