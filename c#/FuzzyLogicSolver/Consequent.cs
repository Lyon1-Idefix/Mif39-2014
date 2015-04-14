using System;
using System.Collections.Generic;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class Consequent : Object
	{
		public Consequent ()
		{
		}
		public void Modify (float activationDegree, Activation activation)
		{
			for (int i = 0; i < conclusions.Count; i ++) {
				Proposition proposition = conclusions[i];
				Activated term = new Activated (proposition.Term,activationDegree,activation);
				OutputVariable output = (OutputVariable) (proposition.Variable);
				output.FuzzyOutput.AddTerm(term);
			}
		}
		public FuzzyLogic.Consequent theConsequent { get; set; }
		public List < Proposition > conclusions = new List < Proposition > ();
	}
}

