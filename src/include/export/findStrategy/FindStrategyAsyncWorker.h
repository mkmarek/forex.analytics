#include "../../../include/Analytics.h"
#include "StrategyUpdateBaton.h"

template <typename T>
static void chromosomeToObject(T* baton, v8::Local<v8::Object>& strategy)
{
	strategy = Nan::New<v8::Object>();
	baton->chromosome->ToJs(strategy);
}

class FindStrategyAsyncWorker : public Nan::AsyncProgressWorker
{
public:
	explicit FindStrategyAsyncWorker(
		Nan::Callback* callback_,
		FindStrategyBaton* baton_)
		: AsyncProgressWorker(callback_)
	{
		this->baton = baton_;
	}

	void Execute(const ExecutionProgress& progress) override
	{
		Analytics system;
		Nan::Callback* callback = this->baton->progress;
		ExecutionProgress* pp = &const_cast<ExecutionProgress&>(progress);

		auto update = [callback, pp](double fitness, BinaryTreeChromosome* chromosome, int generation)
		{
			if (callback != nullptr)
			{
				StrategyUpdateBaton updateBaton;
				updateBaton.fitness = fitness;
				updateBaton.generation = generation;
				updateBaton.chromosome = chromosome;

				pp->Send(reinterpret_cast<const char*>(&updateBaton), sizeof(StrategyUpdateBaton));
			}
		};

		try
		{
			this->baton->chromosome = system.PerformAnalysis(
				baton->closeValues,
				baton->inputTypes,
				baton->input,
				baton->populationCount,
				baton->generationCount,
				baton->selectionAmount,
				baton->leafValueMutationProbability,
				baton->leafSignMutationProbability,
				baton->logicalNodeMutationProbability,
				baton->leafIndicatorMutationProbability,
				baton->crossoverProbability,
				baton->pipInDecimals,
				baton->spread,
				baton->startingChromosome,
				update);
		}
		catch (const char* error)
		{
			baton->errorMessage = error;
			baton->chromosome = nullptr;
		}
	}

	void HandleOKCallback() override {
		Nan::HandleScope scope;

		if (baton->chromosome == nullptr) {

			v8::Handle<v8::Value> argv[] =
			{
				Nan::Undefined(),
				v8::Exception::TypeError(
					Nan::New<v8::String>(baton->errorMessage).ToLocalChecked())
			};

			this->callback->Call(2, argv);
		}
		else {
			v8::Local<v8::Object> strategy;

			chromosomeToObject(baton, strategy);

			v8::Handle<v8::Value> argv[] =
			{
				strategy,
				Nan::Undefined()
			};

			this->callback->Call(2, argv);
		}

		if (baton->progress != nullptr)
			delete baton->progress;

		if (baton->chromosome != nullptr)
			delete baton->chromosome;

		if (baton->startingChromosome != nullptr)
			delete baton->startingChromosome;

		delete baton;
	}

	void HandleErrorCallback() override {
		Nan::HandleScope scope;

		v8::Handle<v8::Value> argv[] =
		{
			Nan::Undefined(),
			v8::Exception::TypeError(
				Nan::New<v8::String>(ErrorMessage()).ToLocalChecked())
		};

		this->callback->Call(2, argv);

		if (baton->progress != nullptr)
			delete baton->progress;

		if (baton->chromosome != nullptr)
			delete baton->chromosome;

		delete baton;
	}

	void HandleProgressCallback(const char* data, size_t size) override
	{
		Nan::HandleScope scope;

		StrategyUpdateBaton* updateBaton = reinterpret_cast<StrategyUpdateBaton*>(const_cast<char*>(data));

		v8::Local<v8::Object> strategy;

		chromosomeToObject(updateBaton, strategy);

		v8::Handle<v8::Value> argv[] =
		{
			strategy,
			v8::Handle<v8::Value>(Nan::New<v8::Number>(updateBaton->fitness)),
			v8::Handle<v8::Value>(Nan::New<v8::Number>(updateBaton->generation)),
		};

		this->baton->progress->Call(3, argv);
	}

private:
	FindStrategyBaton* baton;
};
