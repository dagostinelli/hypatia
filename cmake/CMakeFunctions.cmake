# https://stackoverflow.com/questions/15282444/using-cmake-how-can-i-concat-files-and-install-them
function(cat IN_FILE OUT_FILE)
	file(READ ${IN_FILE} CONTENTS)
	file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

function(deleteinplace IN_FILE pattern)
	file(READ ${IN_FILE} CONTENTS)
	string(REGEX REPLACE "${pattern}" "" STRIPPED "${CONTENTS}")
	file(WRITE ${IN_FILE} "${STRIPPED}")
endfunction()
