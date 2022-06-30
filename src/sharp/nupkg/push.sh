#! /bin/sh
apikey=oy2odmo7qaup5dhya2iqfgexkoqg3syuvlgudn5st4v3ha
source=https://api.nuget.org/v3/index.json
nupkg=`find . -name "*.nupkg"`

nuget push $nupkg $apikey -Source $source
