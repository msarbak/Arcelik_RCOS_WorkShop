set objFSO = CreateObject( "Scripting.FileSystemObject" )

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

function findDevTypes(byval path, byval exception)
    set folder = objFSO.GetFolder(path)
    set files = folder.files
    devTypes = ""
    for each file in files
        if right(file.name, 2) = ".c" then
            if file.name <> exception then
                devTypes = devTypes & left(file.name, len(file.name) - len(".c")) & vbCrLf
            end if
        end if
    next
    findDevTypes = devTypes
end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

function findMiddleWares(byval path)
    set folder = objFSO.GetFolder(path)
    set files = folder.files
    mwTypes = ""
    for each file in files
        if right(file.name, 2) = ".h" then
            mwTypes = mwTypes & left(file.name, len(file.name) - len(".h")) & vbCrLf
        end if
    next
    findMiddleWares = mwTypes
end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' function findCreateMacros(byval folderPath, byval devType)
'     set objFSO = CreateObject( "Scripting.FileSystemObject" )
'     set objFile = objFSO.OpenTextFile(folderPath & devType & ".h", 1)
'     fileContent = ""
'     Do Until objFile.AtEndOfStream
'         fileContent = fileContent & objFile.ReadLine & vbCrLf
'     Loop
'     objFile.Close
' 
'     lines = split(fileContent, vbCrLf)
'     myMacros = ""
'     for each line in lines
'         if instr(line, "define") then
'             if instr(line, "_CREATE(") <> 0 then
'                 line = right(line, len(line) - instr(line, "DEV_") + 1)
'                 line = replace(line, " ", "")
'                 line = replace(line, "\", "")
'                 myMacros = myMacros & line & vbCrLf
'             end if
'         end if
'     next
'     findCreateMacros = myMacros
' end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' function fillCreateMacro(byval createMacro)
'     macroName = left(createMacro, instr(createMacro, "(") - 1)
'     args = right(createMacro, len(createMacro) - instr(createMacro, "("))
'     args = replace(args, ")", "")
'     argsList = split(args, ",")
'     retVal = ""
' 
'     for each argument in argsList
'         argValue = inputbox("Please enter a value for """ & argument & """:" & _
'             vbCrLf & vbCrLf & macroName & "(" & retVal & ")", macroName & " - " & argument)
'         if argValue = "" then
'             if argument = "_name" then
'                 retVal = ", "
'                 exit for
'             else
'                 argValue = argument
'             end if
'         end if
'         retVal = retVal & argValue & ", "
'     next
' 
'     fillCreateMacro = left(retVal, len(retVal) - 2) ' remove ", "
' end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

function findArgDescInParams(byval argName, byval params)
    asdf = split(params, vbCrLf)
    retVal = ""
    for each paramName in asdf
        if instr(paramName, argName) <> 0 then
            retVal = ltrim(right(paramName, len(paramName) - instr(paramName, argName) - len(argName)))
            exit for
        end if
    next
    findArgDescInParams = retVal
end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

function addObjects(byval platformName)
    set objFSO = CreateObject( "Scripting.FileSystemObject" )
    devTypesList = split(devTypes, vbCrLf)
    if platformName <> "" then
        platformName = platformName & "/"
    end if
    do
        msgbox objectsCreate & vbCrLf & vbCrLf & objectsInit, vbOKOnly, "Currently Created Objects!!!"

        deviceType = inputbox( "Please select a device type from list:" & vbCrLf & vbCrLf & devTypes, "dev/" & platformName)
        if deviceType <> "" then
            for each devType in devTypesList
                if lcase(deviceType) = devType then
                    fileContent = ""
                    params = ""
                    ' Place file include line
                    objectsCreate = objectsCreate & _
                        "#include ""dev/" & lcase(platformName) & devType & ".h""" & vbCrLf
                    ' Open target device file
                    set objFile = objFSO.OpenTextFile("./dev/" & lcase(platformName) & devType & ".h", 1)
                    Do Until objFile.AtEndOfStream
                        line = objFile.ReadLine
                        ' Collect parameter descriptions for a creation macro
                        if instr(line, "@param") <> 0 then
                            if instr(line, "_name") then
                                params = ""
                            end if
                            params = params & ltrim(right(line, len(line) - instr(line, "@param") - len("@param"))) & vbCrLf
                            ' params = params & line & vbCrLf
                            ' findArgDescInParams(argName, params)
                        end if
                        ' Process a creation macro
                        if instr(line, "define") <> 0 then
                            if instr(line, "_CREATE(") <> 0 then
                                myArgs = ""
                                ' Separate macro
                                myMacro = right(line, len(line) - instr(line, "DEV_") + 1)
                                myMacro = replace(myMacro, " ", "")
                                myMacro = replace(myMacro, "\", "")
                                ' Separate macro name
                                myMacroName = left(myMacro, instr(myMacro, "(") - 1)
                                ' Separate device class
                                myClass = mid(myMacroName, len("DEV_") + 1, 3)
                                if myClass = "IO_" then
                                    myClass = left(myClass, len(myClass) - 1)
                                end if
                                ' Separate argument names
                                argNames = right(myMacro, len(myMacro) - instr(myMacro, "("))
                                argNames = replace(argNames, ")", "")
                                argArray = split(argNames, ",")
                                ' Process each argument
                                do
                                    msgbox "Entering object creation!" & vbCrLf & "To exit enter a blank name for the object.", vbOKOnly, myMacroName
                                    myArgs = ""
                                    for each argName in argArray
                                        ' findArgDescInParams(argName, params)
                                        argValue = inputbox( params & vbCrLf & vbCrLf & _
                                            myMacroName & "(" & myArgs & "_ )", _
                                            myMacroName & " - " & argName)
                                        if argValue = "" then
                                            if argName = "_name" then
                                                myArgs = ", "
                                                exit for
                                            else
                                                argValue = argName
                                            end if
                                        end if
                                        myArgs = myArgs & argValue & ", "
                                    next
                                    myArgs = left(myArgs, len(myArgs) - len(", ")) ' Remove unnecessary characters
                                    if myArgs <> "" then
                                        myName = left(myArgs, instr(myArgs, ",") - 1)
                                        ' Object creation macro call
                                        objectsCreate = objectsCreate & _
                                            myMacroName & "(" & myArgs & ")" & vbCrLf
                                        ' Object initialization call
                                        select case myClass
                                            case "IO"
                                                objectsInit = objectsInit & _
                                                    "    devIoInit(&" & myName & ", NULL);" & vbCrLf
                                            case "COM"
                                                devComArray = devComArray & vbCrLf & myName
                                                objectsInit = objectsInit & _
                                                    "    devComInit(&" & myName & ");" & vbCrLf
                                            case "MEM"
                                                objectsInit = objectsInit & _
                                                    "    devMemInit(&" & myName & ");" & vbCrLf
                                            case "CPX"
                                                objectsInit = objectsInit & _
                                                    "    devCpxInit(&" & myName & ", NULL);" & vbCrLf
                                        end select
                                    end if
                                loop while myArgs <> ""
                                params = ""
                            end if
                        end if
                    Loop
                    objFile.Close
                    objectsCreate = objectsCreate & vbCrLf
                end if
            next
        end if
    loop while deviceType <> ""
end function

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' START OF SCRIPT
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' Author selection
author = inputbox( "Who is the author?" )

' Platform selection
set devFolder = objFSO.GetFolder("./dev")
set folderArray = devFolder.subfolders
folderNameList = ""
for each folderObj in folderArray
    folderNameList = folderNameList & vbCrLf & folderObj.name
next
platformName = inputbox( "Please select platform:" & vbCrLf & folderNameList )
if not objFSO.folderExists("./dev/" & lcase(platformName)) then
    msgbox "ERROR: UNSUPPORTED PLATFORM"
    wscript.quit
end if

objectsCreate = ""
objectsInit = ""
devComArray = ""

' Create objects from cpu specific device types
if platformName <> "" then
    devTypes = findDevTypes("./dev/" & lcase(platformName), "")
    addObjects(platformName)
end if

' Create objects from generic device types
devTypes = findDevTypes("./dev", "dev.c")
addObjects("")

' Create objects from middlewares
' TODO ' myTypes = findMiddleWares("./mw")

' Event queue size
eventQueueSize = inputbox("Please enter event queue size:")

' Debug selection
debugDevLine = inputbox("Which devCom do you want to use for debug string output(leave empty if not used)?" & vbCrLf & devComArray)
if debugDevLine = "" then
    debugDevLine = "// CORE_DEBUG_DEV(_devName)"
else
    debugDevLine = "CORE_DEBUG_DEV(" & debugDevLine & ")"
end if

' CORE_TICK_DEFAULT
tickArgs = ""

' CORE_WDT_DEFAULT
wdtArgs = ""

' Create "rcos.c"
set objFile = objFSO.OpenTextFile( "_rcos_c", 1 )
template = ""
Do Until objFile.AtEndOfStream
    template = template & objFile.ReadLine & vbCrLf
Loop
objFile.Close

' Place information into "rcos.c"
template = replace( template, "%EVENTQUEUE_SIZE%", eventQueueSize )
template = replace( template, "%DEBUG_DEV_LINE%", debugDevLine )
template = replace( template, "%TICK_ARGS%", tickArgs )
template = replace( template, "%WDT_ARGS%", wdtArgs )
template = replace( template, "%OBJECTS_CREATE%", objectsCreate )
template = replace( template, "%OBJECTS_INIT%", objectsInit )
template = replace( template, "%AUTHOR%", author )
template = replace( template, "%YEAR%", year(now) )
template = replace( template, "%DATE%", formatdatetime( now, vbShortDate ) )

' Save "rcos.c"
Set objFile = objFSO.CreateTextFile( "..\rcos.c", True )
objFile.Write template
objFile.Close

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' Create "rcos.h"
set objFile = objFSO.OpenTextFile( "_rcos_h", 1 )
template = ""
Do Until objFile.AtEndOfStream
    template = template & objFile.ReadLine & vbCrLf
Loop
objFile.Close

' Place information into "rcos.h"
template = replace( template, "%PLATFORM%", ucase( platformName ) )
template = replace( template, "%PLATFORM_LC%", lcase( platformName ) )
template = replace( template, "%AUTHOR%", author )
template = replace( template, "%YEAR%", year(now) )
template = replace( template, "%DATE%", formatdatetime( now, vbShortDate ) )

' Save "rcos.h"
Set objFile = objFSO.CreateTextFile( "..\rcos.h", True )
objFile.Write template
objFile.Close

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' Create folders and copy files
fileNameList = ""_
    & "./app/_createApp.vbs"_
    & "./app/_tempApp_c"_
    & "./app/_tempApp_h"_
    & "./dev/_createDevice.vbs"_
    & "./dev/_tempDevCom_c"_
    & "./dev/_tempDevCom_h"_
    & "./dev/_tempDevCpx_c"_
    & "./dev/_tempDevCpx_h"_
    & "./dev/_tempDevIo_c"_
    & "./dev/_tempDevIo_h"_
    & "./dev/_tempDevMem_c"_
    & "./dev/_tempDevMem_h"

fileNameArray = split(fileNameList, "./")
for each fileName in fileNameArray
    if len(fileName) > 0 then
        folderName = left(fileName, instr(fileName, "/"))
        if not objFSO.folderExists("../" & folderName) then
            objFSO.CreateFolder("../" & folderName)
        end if
        if objFSO.fileExists("./" & fileName) then
            objFSO.copyfile "./" & fileName, "../" & fileName
        end if
    end if
next
