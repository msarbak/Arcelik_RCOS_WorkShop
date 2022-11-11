Set objFSO = CreateObject( "Scripting.FileSystemObject" )

' Enter parameters
templateName = "_tempApp"
if not objFSO.fileexists(templateName&"_c") then
msgbox "ERROR: TEMPLATE DOES NOT EXISTS"
wscript.quit
end if
fileName = inputbox( "What is the name of the process?" & vbCrLf & "example: camel_Case" )
if fileName = "" then
msgbox "ERROR: EMPTY FILE NAME"
wscript.quit
end if
if len(fileName) < 3 then
msgbox "ERROR: SHORT FILE NAME"
wscript.quit
end if
appName = replace( fileName, "_", "" )
briefDesc = inputbox( "What is it about?" )
author = inputbox( "Who is the author?" )

debName = inputbox("Please input a short name to be used for debug logs:")
debPortsArray = ""
idx = 0
Do
    debPortName = inputbox("Please define a debug port for this file(leave empty to exit):")
    if debPortName <> "" then
        debPortsArray = debPortsArray & "#define DEBUG_PORT_" & ucase(debPortName) & " BIT(" & idx & ")" & vbCrLf
    end if
    idx = idx + 1
loop while debPortName <> ""

' Create C file
Set objFile = objFSO.OpenTextFile( templateName & "_c", 1 )
template = ""
Do Until objFile.AtEndOfStream
    template = template & objFile.ReadLine & vbCrLf
Loop
objFile.Close

template = replace( template, "%FILENAMELC%", lcase( fileName ) )
template = replace( template, "%FILENAMEUC%", ucase( fileName ) )
template = replace( template, "%DEBUG_NAME%", debName )
template = replace( template, "%DEBUG_PORTS%", debPortsArray )
template = replace( template, "%APPNAME%", appName )
template = replace( template, "%APPNAMEPC%", ucase( left(appName,1) ) & right(appName, len(appName) - 1 ) )
'template = replace( template, "%FILENAMEPC%", ucase( left(fileName,1) ) & lcase( right(fileName, len(fileName) - 1 ) ) )
template = replace( template, "%AUTHOR%", author )
template = replace( template, "%BRIEF%", briefDesc )
template = replace( template, "%YEAR%", year(now) )
template = replace( template, "%DATE%", formatdatetime( now, vbShortDate ) )

Set objFile = objFSO.CreateTextFile( lcase( fileName ) & ".c", True )
objFile.Write template
objFile.Close


' Create H file
Set objFile = objFSO.OpenTextFile( templateName & "_h", 1 )
template = ""
Do Until objFile.AtEndOfStream
    template = template & objFile.ReadLine & vbCrLf
Loop
objFile.Close

template = replace( template, "%FILENAMELC%", lcase( fileName ) )
template = replace( template, "%FILENAMEUC%", ucase( fileName ) )
template = replace( template, "%APPNAME%", appName )
template = replace( template, "%APPNAMEPC%", ucase( left(appName,1) ) & right(appName, len(appName) - 1 ) )
'template = replace( template, "%FILENAMEPC%", ucase( left(fileName,1) ) & lcase( right(fileName, len(fileName) - 1 ) ) )
template = replace( template, "%AUTHOR%", author )
template = replace( template, "%BRIEF%", briefDesc )
template = replace( template, "%YEAR%", year(now) )
template = replace( template, "%DATE%", formatdatetime( now, vbShortDate ) )

Set objFile = objFSO.CreateTextFile( lcase( fileName ) & ".h", True )
objFile.Write template
objFile.Close
