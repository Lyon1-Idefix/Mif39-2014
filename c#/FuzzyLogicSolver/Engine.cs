using System;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class Engine : Object
	{
		public Engine ()
		{
		}
		public void Configure ( Conjunction conjunction, Disjunction disjunction, Activation activation, Accumulation accumulation, Defuzzyfier defuzzyfier )
		{
			foreach (RuleBlock block in theBlocks) {
				block.Configure ( conjunction, disjunction, activation );
			}
			foreach (OutputVariable output in theOutputs) {
				output.Configure ( accumulation, defuzzyfier );
			}
		}
		public void Process ()
		{
			foreach (OutputVariable output in theOutputs)
				output.FuzzyOutput.Clear ();
			foreach (RuleBlock block in theBlocks)
				block.Activate ();
			foreach (OutputVariable output in theOutputs)
				output.DeFuzzify ();
		}
		//
		public FuzzyLogic.Engine theEngine { get; set; }
		public InputList theInputs = new InputList ();
		public OutputList theOutputs = new OutputList ();
		public RuleBlockList theBlocks = new RuleBlockList ();
	}
}

