#  -*- coding: utf-8 -*-
#    @package: disciplinas-csv.py
#     @author: Cristian C.
#	 @version: 0.1
#
from mwebcrawler import Cursos, Disciplina as mwDisciplina, Nivel
import json
import csv

idConversionEnabled = True and False

verboseEnabled = True #and False
def printVerbose(str):
	if (verboseEnabled):
		print(str)


class Disciplina ():
	codigo = None
	nome = None
	creditos = None
	fator = None
	requisitos = None

	def __init__(self, codigo):
		self.codigo = codigo

	def getPeso():
		if (creditos == None or fator == None):
			return False
		return creditos*fator 

class Main():
	fluxoFileName = 'cic_fluxo.csv'
	curriculoFileName = 'cic_curriculo.txt'

	@staticmethod
	def curriculoCrawler(opcao='1856'): #Ciência da Comp.
		try:
			outFile = open(Main.fluxoFileName, 'w', newline='') #py2.7 needs to open as wb (http://stackoverflow.com/a/3348664)
		except:
			print('(E) Nao foi possível abrir arquivo de saída ({0}).'.format(Main.fluxoFileName))
			return False

		curriculo = Cursos.curriculo(opcao, nivel=Nivel.GRADUACAO, verbose=False)

		#caso base onde somente disciplinas obrigatorias são consideradas
		obg = curriculo['optativas']
		if (len(obg) == 0):
			print('(E) Falha ao exportar disciplinas. Verifique o código do curso ({0})'\
				' e/ou a conexao com a internet.'.format(opcao)\
				)
			return False

		disciplinaList = []; idx = 0;
		cod_toID = {}
		for cod in obg:

			cre_info = obg[cod]['Créditos']
			nom_info = obg[cod]['Nome']

			cre_total = 0
			cre_total += cre_info['Teoria']
			cre_total += cre_info['Prática']

			req_info = mwDisciplina.pre_requisitos(cod, nivel=Nivel.GRADUACAO, verbose=False)
			reqList = []
			if (len(req_info)>0):
				reqList = req_info[0] # ignorar [[cod,cod...] OU [cod,...]]

			newDisciplina = Disciplina(cod)
			newDisciplina.creditos = cre_total
			newDisciplina.nome = nom_info
			newDisciplina.fator = 0
			newDisciplina.requisitos = reqList
			disciplinaList.append(newDisciplina);

			cod_toID[cod] = idx
			idx += 1

		#Export to csv
		csvWriter = csv.writer(outFile, dialect='excel')

		csvWriter.writerow(['CODIGO (Cod)', 'NOME (N)', 'CREDITOS (Cr)', 'FATOR (f)', 'REQUISITOS...'])

		print('(I) Encontrada(s) {0} disciplinas.'.format(len(disciplinaList)))
		for disc in disciplinaList:
			req = disc.requisitos
			if (idConversionEnabled):
				req = []
				for cod in disc.requisitos:
					try:
						req.append(cod_toID[cod])
					except KeyError: #semantically this should not happen but just in case
						req.append(-1)

			csvWriter.writerow([disc.codigo, disc.nome, disc.creditos, ''] + req)

		outFile.close()
		return True

	@staticmethod
	def test_curriculoCrawler():
		opcao = '1856' # Ciência da Comp.
		curriculo = Cursos.curriculo(opcao, nivel=Nivel.GRADUACAO, verbose=False)

		jsonOutput = json.dumps(curriculo, ensure_ascii=False) #15.8 KB de dados para opção 1856
		outFile = open(Main.curriculoFileName, 'w')

		outFile.write(jsonOutput)
		outFile.close()

	@staticmethod
	def test_fluxoCrawler():
		opcao = '1856' # Ciência da Comp.
		fluxo = Cursos.fluxo(opcao, nivel=Nivel.GRADUACAO, verbose=False)

		numSemestres = len(fluxo)
		outFile = open(Main.fluxoFileName, 'w')

		csvText = str(numSemestres) + '\n';
		for idx in fluxo:
			line = ""
			for disciplina_cod in fluxo[idx]['Disciplinas']:
				line = line + str(disciplina_cod) + ', '
			csvText = csvText + line[:-2] + '\n'
		outFile.write(csvText)
		outFile.close()


def scriptMain ():
	Main.curriculoCrawler()

if __name__ == "__main__":
    scriptMain()