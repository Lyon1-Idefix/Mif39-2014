using System;
using System.Collections.Generic;

namespace FuzzyLogicSolver
{
	public static class Converter
	{
		public static Engine Convert ( FuzzyLogic.Engine flObject ) {
			Engine result = new Engine ();
			result.theEngine = flObject;
			foreach (FuzzyLogic.InputVariable var in flObject.Inputs)
				result.theInputs.Add (Convert (var));
			foreach (FuzzyLogic.OutputVariable var in flObject.Outputs)
				result.theOutputs.Add (Convert (var));
			foreach (FuzzyLogic.RuleBlock var in flObject.Blocks)
				result.theBlocks.Add (Convert (var));
			return result;
		}
		static Curve Convert ( FuzzyLogic.Curve flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (Curve) converted [flObject.ID];
			Curve result = new Curve ();
			result.theCurve = flObject;
			converted [flObject.ID] = result;
			return result;
		}
		static Variable Convert ( FuzzyLogic.Variable flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (Variable) converted [flObject.ID];
			Variable result = null;
			if ( flObject is FuzzyLogic.InputVariable )
				result = Convert ( (FuzzyLogic.InputVariable) flObject );
			else if ( flObject is FuzzyLogic.OutputVariable )
				result = Convert ( (FuzzyLogic.OutputVariable) flObject );
			converted [flObject.ID] = result;
			return result;
		}
		static InputVariable Convert ( FuzzyLogic.InputVariable flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (InputVariable) converted [flObject.ID];
			InputVariable result = new InputVariable ();
			result.theVariable = flObject;
			foreach (FuzzyLogic.Curve curve in flObject.Terms)
				result.Terms.Add (Convert (curve));
			result.theInput = flObject;
			converted [flObject.ID] = result;
			return result;
		}
		static OutputVariable Convert ( FuzzyLogic.OutputVariable flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (OutputVariable) converted [flObject.ID];
			OutputVariable result = new OutputVariable ();
			result.theVariable = flObject;
			foreach (FuzzyLogic.Curve curve in flObject.Terms)
				result.Terms.Add (Convert (curve));
			result.theOutput = flObject;
			result.FuzzyOutput = new Accumulated (flObject.Name, flObject.Minimum, flObject.Maximum);
			converted [flObject.ID] = result;
			return result;
		}
		static RuleBlock Convert ( FuzzyLogic.RuleBlock flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (RuleBlock) converted [flObject.ID];
			RuleBlock result = new RuleBlock ();
			result.theBlock = flObject;
			foreach (FuzzyLogic.Rule rule in flObject.Conclusions)
				result.Conclusions.Add (Convert (rule));
			converted [flObject.ID] = result;
			return result;
		}
		static Rule Convert ( FuzzyLogic.Rule flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (Rule) converted [flObject.ID];
			Rule result = new Rule ();
			result.theRule = flObject;
			result.antecedent = Convert (flObject.Antecedent);
			result.consequent = Convert (flObject.Consequent);
			converted [flObject.ID] = result;
			return result;
		}
		static Antecedent Convert ( FuzzyLogic.Antecedent flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (Antecedent) converted [flObject.ID];
			Antecedent result = new Antecedent ();
			result.theAntecedent = flObject;
			result.Expression = Convert (flObject.Expression);
			converted [flObject.ID] = result;
			return result;
		}
		static Consequent Convert ( FuzzyLogic.Consequent flObject ) {
			if (converted.ContainsKey (flObject.ID))
				return (Consequent) converted [flObject.ID];
			Consequent result = new Consequent ();
			result.theConsequent = flObject;
			foreach (FuzzyLogic.Proposition prop in flObject.Conclusions)
				result.conclusions.Add (Convert (prop));
			converted [flObject.ID] = result;
			return result;
		}
		static Expression Convert (FuzzyLogic.Expression flObject)
		{
			if (converted.ContainsKey (flObject.ID))
				return (Expression) converted [flObject.ID];
			Expression result = null;
			if (flObject is FuzzyLogic.Operator)
				result = Convert ((FuzzyLogic.Operator)flObject);
			else if (flObject is FuzzyLogic.Proposition)
				result = Convert ((FuzzyLogic.Proposition)flObject);
			result.theExpression = flObject;
			converted [flObject.ID] = result;
			return result;
		}
		static Operator Convert (FuzzyLogic.Operator flObject)
		{
			if (converted.ContainsKey (flObject.ID))
				return (Operator) converted [flObject.ID];
			Operator result = new Operator ();
			result.theOperator = flObject;
			result.Left = Convert (flObject.Left);
			result.Right = Convert (flObject.Right);
			converted [flObject.ID] = result;
			return result;
		}
		static Proposition Convert (FuzzyLogic.Proposition flObject)
		{
			if (converted.ContainsKey (flObject.ID))
				return (Proposition) converted [flObject.ID];
			Proposition result = new Proposition ();
			result.theProposition = flObject;
			result.Variable = Convert (flObject.Variable);
			result.Term = Convert (flObject.Term);
			converted [flObject.ID] = result;
			return result;
		}
		static Dictionary < int, Object > converted = new Dictionary<int, Object> ();
	}
}

