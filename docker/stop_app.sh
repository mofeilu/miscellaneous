docker rm $(docker stop $(docker ps -a -q --filter ancestor=my_app))
