/*
TODO

refactors
---
get things out of encoding.js
get checks out of sanityChecks

Vue cleanup
---
congrats message if there are no problems at all
one accordion component
slots / pass children for innermost's content
- change other uses of accordion to my accordion?

blacklisting
---
maps.json strategy? would be map-level only

CLI print-out
---
--verbose arg?

definitions checking
---
implement for JS
check for script definitions in JSON files as well as MGS files (there are a couple)

misc
---
documentation for API (e.g., return null or a string error message)
ask about scripts.js: var possibleEntityScripts = [ 'on_interact', 'on_tick', 'on_look', ];
ask about populating sanityChecks onto scenarioData
presentation design
- original design: a report like the python output that would be useful to download
	- fixes separate from problems
- new design: Vue component for each problem
	- fixes presented next to problems
problem counts for GUI and CLI
- final print-out info in GUI?
txt download next to copy button? (ask)
how will look work for multiple entities of the same or similar names (eg bread, torch)
gameengine prettier mage_command_control.cpp:185
- commandResponseBuffer += "\"" + subject + "\" is not a valid entity name.\n";
ask Mary for a pattern for "TODO" script definitions
- <m> true names
- newline then space at end?
move look scripts not in the look scripts file? e.g. see ch2-castle-34.mgs
- (also naming of script look-ch2-castle-34 but it's really for the pantry?)
use a predicate function for blacklisted_files? e.g. positive string match 'ch2'

generated fixes
---
generating script names
- nice indenting to be able to paste into maps
- what to do with a no-name entity?
- check if name we generate was already used (like above, probably good enough to output to a separate section needing manual fixing)
use fallback entity names that are generated somewhere late in the build process for fixes? (e.g. 'Mage 32')
- are these only showing up sometimes? 
*/







Vue.component('editor-sanity-check-problem', {
	name: 'editor-sanity-check-problem',
	props: {
		problem: {
			type: Object,
			required: true
		},
		fix: {
			type: String,
			required: false,
			default: '{\n    json: {\n        morejson: {\n            \'TODO generated fixes\'\n        }\n    }\n}'
		}
	},
	data: function () {
		return {
			collapsed: true
		}
	},
	methods: {
		collapse: function () {
			this.collapsed = !this.collapsed;
		},
		copyFixes: function () {
			this.$refs.copyFixesTextArea.select();
			document.execCommand('copy');
		}
	},
	template: /*html*/`
<div class="card mb-1 text-white">
	<div class="card-header bg-primary">
		{{ problem.name || "NO NAME" }} (id {{ problem.id }})
		<span
			class="position-absolute"
			style="top: 6px; right: 6px;"
		>
			<button
				type="button"
				class="btn btn-outline-info"
				@click="collapse"
			>_</button>
		</span>
	</div>
	<div
		class="card-body p-3"
		v-if="!collapsed"
	>
		<p>{{ problem.problemMessage }}</p>
		
	
	


		<div
			class="alert alert-info"
			role="alert"
		>
			<span>You can click the "Copy" button to the right to put the current TODO dynamic your clipboard, then paste it into your "<strong>TODO dynamic</strong>" file to save.</span>
		</div>

		<p>You can click the button to the right to copy these suggested fixes.</p>
		<div class="row align-items-start flex-nowrap">
			<pre class="border border-primary rounded p-2 w-100">{{ fix }}</pre>
			<button
				type="button"
				class="ml-1"
				style="width: 2rem;"
				title="Copy"
				@click="copyFixes"
			>
				<span aria-hidden="true">📋</span>
			</button>
		</div>

		<textarea
			cols="80"
			rows="16"
			class="position-absolute"
			style="
				font-size: 0;
				opacity: 0;
			"
			ref="copyFixesTextArea"
		>{{ fix }}</textarea>
	</div>
</div>
`});












Vue.component('editor-sanity-check-map', {
	name: 'editor-sanity-check-map',
	props: {
		mapName: {
			type: String,
			required: true
		},
		problems: {
			type: Array,
			required: true
		}
	},
	data: function () {
		return {
			collapsed: true
		}
	},
	methods: {
		collapse: function () {
			this.collapsed = !this.collapsed;
		}
	},
	template: /*html*/`
<div class="card mb-1 text-white">
	<div class="card-header bg-primary">
		Problems in map '{{ mapName }}' ({{ problems.length }} entities)
		<span
			class="position-absolute"
			style="top: 6px; right: 6px;"
		>
			<button
				type="button"
				class="btn btn-outline-info"
				@click="collapse"
			>_</button>
		</span>
	</div>
	<div
		class="card-body p-3"
		v-if="!collapsed"
	>
		<editor-sanity-check-problem
			v-for="problem in problems"
			:key="problem.id"
			:problem="problem"
		></editor-sanity-check-problem>
	</div>
</div>
`});








Vue.component('editor-sanity-check', {
	name: 'editor-sanity-check',
	props: {
		checkName: {
			type: String,
			required: true
		},
		maps: {
			type: Object,
			required: true
		}
	},
	data: function () {
		return {
			collapsed: true
		}
	},
	methods: {
		collapse: function () {
			this.collapsed = !this.collapsed;
		}
	},
	template: /*html*/`
<div class="card mb-1 text-white">
	<div class="card-header bg-primary">
		Problems with '{{ checkName }}' ({{ Object.keys(maps).length }} maps)
		<span
			class="position-absolute"
			style="top: 6px; right: 6px;"
		>
			<button
				type="button"
				class="btn btn-outline-info"
				@click="collapse"
			>_</button>
		</span>
	</div>
	<div
		class="card-body p-3"
		v-if="!collapsed"
	>
		<editor-sanity-check-map
			v-for="(problems, mapName) in maps"
			:key="mapName"
			:map-name="mapName"
			:problems="problems"
		></editor-sanity-check-map>
	</div>
</div>
`});








Vue.component('editor-sanity-checks', {
	name: 'editor-sanity-checks',
	props: {
		problems: {
			type: Object,
			required: true
		}
	},
	data: function () {
		return {
			collapsed: true
		}
	},
	methods: {
		collapse: function () {
			this.collapsed = !this.collapsed;
		}
	},
	template: /*html*/`
<div class="card mb-3 text-white">
	<div class="card-header">
		Additional reports about the build
		<span
			class="position-absolute"
			style="top: 6px; right: 6px;"
		>
			<button
				type="button"
				class="btn btn-outline-light"
				@click="collapse"
			>_</button>
		</span>
	</div>
	<div
		class="card-body p-3"
		v-if="!collapsed"
	>
		<template v-if="Object.keys(problems).length">
			<editor-sanity-check
				v-for="(maps, checkName) in problems"
				:key="checkName"
				:check-name="checkName"
				:maps="maps"
			></editor-sanity-check>
		</template>
		<div v-else>
			<img src="./dependencies/MageDance.gif"/>
			<span class="mx-1 align-bottom">None. How fastidious of you.</span>
		</div>
	</div>
</div>`});
