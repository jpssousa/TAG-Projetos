'Launches an custom URL with the chosen user input
'Author  : Cristian C.

Set WshShell = CreateObject("WScript.Shell")
Dim URL
URL = Inputbox("Codigo do Curso: ")
If Len(URL) = 0 Then
	Wscript.Quit
End If
URL = "https://matriculaweb.unb.br/graduacao/disciplina_pop.aspx?cod=" & URL

WshShell.run(URL)  'source: http://stackoverflow.com/a/13401872