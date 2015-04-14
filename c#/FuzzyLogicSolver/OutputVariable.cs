using System;
using System.Collections.Generic;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class OutputVariable : Variable
	{
		public OutputVariable ()
		{
		}
		public void Configure ( Accumulation accumulation, Defuzzyfier defuzzyfier )
		{
			Defuzzyfier = defuzzyfier;
			FuzzyOutput.Accumulation = accumulation;
		}
		public void DeFuzzify ()
		{
			float result = float.NaN;
			result = Defuzzyfier.defuzzify (FuzzyOutput, theOutput.Minimum, theOutput.Maximum);
			// TODO: in range test
			theOutput.Value = result;
		}
		//
		public FuzzyLogic.OutputVariable theOutput { get; set; }
		public Defuzzyfier Defuzzyfier { get; set; }
		public Accumulated FuzzyOutput { get; set; }
	}
	public class OutputList : List < OutputVariable > {}
}

