using System;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;
using Defuzzyfier = FuzzyLogicSolver.Defuzzyfier;

namespace FuzzyLogicSolver
{
	public class Antecedent : Object
	{
		public Antecedent ()
		{
		}
		public float ActivationDegree (Conjunction conjunction, Disjunction disjunction, Expression expr)
		{
			//Console.WriteLine ("Computing Antecedent activation degree");
			float result = Single.NaN;
			if (expr is Proposition) {
				//Console.WriteLine ("Antecedent is a proposition");
				Proposition prop = (Proposition)expr;
				if (prop.Variable is InputVariable) {
					//Console.WriteLine ("With input variable: " + ((InputVariable)prop.Variable).theInput.Name + " = " + ((InputVariable)prop.Variable).theInput.Value );
					result = prop.Term.Membership (((InputVariable)prop.Variable).theInput.Value);
				} else if (prop.Variable is OutputVariable) {
					//Console.WriteLine ("With output variable");
					result = ((OutputVariable)prop.Variable).FuzzyOutput.ActivationDegree (prop.Term);
				}
			} else if (expr is Operator) {
				//Console.WriteLine ("Antecedent is an operator");
				Operator fuzzyOperator = (Operator) expr;
				if (fuzzyOperator.theOperator.OperatorName == "AND") {
					result = conjunction.Compute ( ActivationDegree ( conjunction, disjunction, fuzzyOperator.Left ), ActivationDegree ( conjunction, disjunction, fuzzyOperator.Right ) );
				}
				else if (fuzzyOperator.theOperator.OperatorName == "OR") {
					result = disjunction.Compute ( ActivationDegree ( conjunction, disjunction, fuzzyOperator.Left ), ActivationDegree ( conjunction, disjunction, fuzzyOperator.Right ) );
				}
			}
			return result;
		}
		public float ActivationDegree (Conjunction conjunction, Disjunction disjunction)
		{
			return ActivationDegree (conjunction, disjunction, Expression);
		}
		public FuzzyLogic.Antecedent theAntecedent { get; set; }
		public Expression Expression { get; set; }
	}
}

