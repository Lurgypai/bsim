Requires:
    libSGP4 installed to ./sgp4-install
    sfml
    cmake

Setup:
    install sfml from command line (apt install libsfml-dev)
    compile libSGP4 and place in local directory under ./sgp4-install
    use the out.sh script to generate cmake files (run ./out.sh)
    cd into the out dir and make (cd out && make)
    copy the hypatia data to the out directory (needs ground_stations.txt, tles.txt, isls.txt and all routes (which are each labelled as fstate_<time>.txt) in a folder called ./routes) data provided in ./datafiles

Running
    run ./bsim_3d
    Controls are
            e
          s d f
    For up down left right rotation.
    
    
