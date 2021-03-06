CONTAINER_ID="$(docker ps | grep "bdd" | cut -d " " -f 1)"
docker kill $CONTAINER_ID
docker rm $CONTAINER_ID

IMAGE_ID="$(docker images | grep "bdd" | xargs | cut -d " " -f 3 | xargs)"
docker rmi $IMAGE_ID

docker build . -t bdd
docker run -d -e "MIN=$1" -e "MAX=$2" -e "FNCOUNT=$3" -e "JOBSCOUNT=$4" --rm -v $(pwd)/measurement:/app/measurement bdd