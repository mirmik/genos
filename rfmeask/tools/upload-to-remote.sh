
cd ..
ssh rfmeas@$1 rm -rf ~/rfmeask
scp -r rfmeask rfmeas@$1:~/rfmeask